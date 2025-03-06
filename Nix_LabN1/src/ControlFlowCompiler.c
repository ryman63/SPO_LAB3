#include "ControlFlowCompiler.h"

void compile(CallGraphNode* callGraph) {
    initVM(&_virtualMachine);
    initSymbolTable(&_symbolTable);
    initCallStack(&_callStack);

    traverseCallGraph(callGraph);

    size_t countInstr = calculateCountInstr();

    Instruction* programLinearCode = malloc(sizeof(Instruction) * countInstr);

    loadProgram(programLinearCode, blocks, counter_blocks);

    runVM(&_virtualMachine, programLinearCode, countInstr);

    free(programLinearCode);

    // free всех структур
}

size_t calculateCountInstr() {
    size_t resultCount = 0;
    for (size_t i = 0; i < counter_blocks; i++) {
        resultCount += blocks[i]->countInstructions;
    }
    return resultCount;
}

void traverseCallGraph(CallGraphNode* root) {
    if (root == NULL) return;

    // Генерируем код для текущей функции
    generateFunctionCode(root->unit);

    // Рекурсивно вызываем для детей
    for (size_t i = 0; i < root->children->size; i++) {
        CallGraphNode* child = getItem(root->children, i);
        traverseCallGraph(child);
    }
}

void generateFunctionCode(ProgramUnit* unit) {

    if (findSymbol(&_symbolTable, unit->funcSignature->name))
    {
        // обработка ошибки

        return;
    }

    addSymbol(&_symbolTable, unit->funcSignature->name, SYMBOL_FUNCTION, 0, 0, unit->funcSignature->returnType);

    // создаём блок для пролога функции
    BasicBlock* prologueBlock = createBasicBlock();

    pushFrame(&_callStack, unit);

    // создание метки
    addInstruction(prologueBlock, createInstruction(OC_MARK, 0, 0, 0, NULL, unit->funcSignature->name));

    // Пролог функции: сохраняем контекст
    // Выгружаем аргументы функции в регистры из стека
    for (size_t i = 0; i < unit->funcSignature->funcArgs->size; i++) {
        FuncArg* arg = (FuncArg*)getItem(unit->funcSignature->funcArgs, i);
        int reg = allocateRegister(&_virtualMachine.allocator);
        addInstruction(prologueBlock, createInstruction(OC_POP, reg, 0, 0, NULL, NULL));
        Symbol* argSymbol = findSymbol(unit->currentTable, arg->name);
        if (argSymbol) {
            argSymbol->location = LOC_REG;
            argSymbol->address = reg;
        }
        else {
            // обработка ошибки
            printf("Не найден аргумент функции");
        }
    }

    //помещаем блок пролога в массив
    blocks[counter_blocks] = prologueBlock;
    counter_blocks++;
    reg returnReg = -1;
    returnReg = traverseCfg(unit->cfg, unit->currentTable, returnReg);

    // Завершаем функцию инструкцией возврата и пушим последний вычисленный элемент
    BasicBlock* finalBlock = createBasicBlock();

    if(unit->funcSignature->returnType != TYPE_VOID)
        addInstruction(finalBlock, createInstruction(OC_PUSHR, 0, returnReg, 0, NULL, NULL));

    if(strcmp(unit->funcSignature->name, "main") == 0)
        addInstruction(finalBlock, createInstruction(OC_HALT, 0, 0, 0, NULL, NULL));
    else
        addInstruction(finalBlock, createInstruction(OC_RET, 0, 0, 0, NULL, NULL));
   
    blocks[counter_blocks] = finalBlock;
    counter_blocks++;

    // Освобождаем регистры после завершения работы функции
    for (size_t i = 0; i < NUM_REGISTERS; i++) {
        freeRegister(&_virtualMachine.allocator, i);
    }
}

reg traverseCfg(CfgNode* cfg, SymbolTable* table, reg returnReg) {
    BasicBlock* block = NULL;
    if (cfg->opTree) {
        block = createBasicBlock();

        returnReg = generateOpTreeCode(cfg->opTree, block, table);

        blocks[counter_blocks] = block;
        counter_blocks++;
    }

    if (cfg->condJump) {
        returnReg = traverseCfg(cfg->condJump, table, returnReg);
    }
    if (cfg->uncondJump) {
        returnReg = traverseCfg(cfg->uncondJump, table, returnReg);
    }
    return returnReg;
}

reg generateFunctionCall(OpNode* opNode, BasicBlock* block, SymbolTable* table) {
    const int FUNC_NAME_INDEX = 0;

    OpNode* funcNameOpNode = getItem(opNode->args, FUNC_NAME_INDEX);

    // Генерация кода передачи аргументов
    for (size_t i = FUNC_NAME_INDEX + 1; i < opNode->args->size; i++) {
        OpNode* funcArg = getItem(opNode->args, i);
        reg reg = generateOpTreeCode(funcArg, block, table);
        addInstruction(block, createInstruction(OC_PUSHR, 0, reg, 0, NULL, NULL));
    }

    // Вызов функции
    addInstruction(block, createInstruction(OC_CALL, 0, 0, 0, funcNameOpNode->value, NULL));

    reg returnReg = allocateRegister(&_virtualMachine.allocator);
    addInstruction(block, createInstruction(OC_POP, returnReg, 0, 0, NULL, NULL));

    return returnReg;
}

reg generateBinaryOpCode(OpNode* opNode, BasicBlock* block, SymbolTable* table) {
    OpNode* left = getItem(opNode->args, 0);
    OpNode* right = getItem(opNode->args, 1);

    reg destReg = allocateRegister(&_virtualMachine.allocator);

    reg regSrc1 = generateOpTreeCode(left, block, table);
    reg regSrc2 = generateOpTreeCode(right, block, table);

    if (!strcmp(opNode->value, "+")) {
        addInstruction(block, createInstruction(OC_ADD, destReg, regSrc1, regSrc2, NULL, NULL));
    }
    else if (!strcmp(opNode->value, "-")) {
        addInstruction(block, createInstruction(OC_SUB, destReg, regSrc1, regSrc2, NULL, NULL));
    }
    else {
        // неизвестная бинарная операция
    }

    //freeRegister(&_virtualMachine.allocator, regSrc1);
    //freeRegister(&_virtualMachine.allocator, regSrc2);
    return destReg;
}

int32_t generateStackPlaceCode(OpNode* opNode, BasicBlock* block, SymbolTable* table, reg src) {
    Symbol* symbol = findSymbol(table, opNode->value);

    if (symbol->address != -1) {
        return symbol->address;
    }

    int typeSize = getTypeSize(opNode->valueType);

    if (typeSize <= 0) {
        printf("Error: Unknown type\n");
    }

    char* buff = malloc(sizeof(char) * 16);

    _virtualMachine.sp += typeSize;
    size_t relativeAddress = _virtualMachine.sp;


    _itoa_s(typeSize, buff, 16, 10);
    addInstruction(block, createInstruction(OC_SUBI, sp, sp, 0, buff, NULL));
    symbol->address = relativeAddress;
    symbol->location = LOC_STACK;

    char* valuePlaceAddress = malloc(sizeof(char) * 16);

    _itoa_s(relativeAddress, valuePlaceAddress, 16, 10);

    addInstruction(block, createInstruction(OC_MOVI, 0, src, 0, valuePlaceAddress, NULL));

    return relativeAddress;
}

void generateRegPlaceCode(OpNode* opNode, BasicBlock* block, SymbolTable* table, reg dest, reg src) {
    Symbol* symbol = findSymbol(table, opNode->value);

    if (symbol->address != -1) {
        freeRegister(&_virtualMachine.allocator, dest);
        dest = symbol->address;
    }

    symbol->address = dest;
    symbol->location = LOC_REG;

    int typeSize = getTypeSize(opNode->valueType);

    if (typeSize <= 0) {
        printf("Error: Unknown type\n");
    }

    addInstruction(block, createInstruction(OC_MOV, dest, src, 0, NULL, NULL));

    freeRegister(&_virtualMachine.allocator, src);
}

reg generateReadOpCode(OpNode* opNode, BasicBlock* block, SymbolTable* table) {

    OpNode* arg = getItem(opNode->args, 0);

    StackFrame frame = peekFrame(&_callStack);

    Symbol* symbol = findSymbol(table, arg->value);

    if (!symbol) {
        // обработка ошибки - переменная не инициализирована

        return;
    }

    switch (symbol->location)
    {
    case LOC_REG: {
        return symbol->address;
    } break;
    case LOC_STACK: {
        reg reg = allocateRegister(&_virtualMachine.allocator);

        char* address = malloc(sizeof(char) * 16);

        _itoa_s(symbol->address, address, 16, 10);

        addInstruction(block, createInstruction(OC_IMOV, reg, 0, 0, address, NULL));

        return reg;
    } break;
    }

    return -1; // Возвращаем регистр с загруженным значением
}

reg generateSetOpCode(OpNode* opNode, BasicBlock* block, SymbolTable* table) {
    const int L_VALUE_INDEX = 0;
    const int R_VALUE_INDEX = 1;

    OpNode* lValueOpNode = getItem(opNode->args, L_VALUE_INDEX);
    OpNode* rValueOpNode = getItem(opNode->args, R_VALUE_INDEX);

    reg reg2 = generateOpTreeCode(rValueOpNode, block, table);

    if (lValueOpNode->opType == OT_PLACE) {
        Symbol* symbol = findSymbol(table, lValueOpNode->value);

        int32_t reg3 = allocateRegister(&_virtualMachine.allocator);

        if (reg3 >= 0) {
            generateRegPlaceCode(lValueOpNode, block, table, reg3, reg2);
            return reg3;
        }
        else {
            return generateStackPlaceCode(lValueOpNode, block, table, reg2);
        }
        
    }
    else {

    }
    return reg2;
}

int32_t generateConstOpCode(OpNode* opNode, BasicBlock* block, SymbolTable* table) {
    const int INDEX_CONST_VALUE = 0;
    OpNode* constValueOpNode = getItem(opNode->args, INDEX_CONST_VALUE);

    int32_t reg = allocateRegister(&_virtualMachine.allocator);
    addInstruction(block, createInstruction(OC_LOAD, reg, 0, 0, constValueOpNode->value, NULL));

    return reg;
}

reg generateOpTreeCode(OpNode* opNode, BasicBlock* block, SymbolTable* table) {
    switch (opNode->opType) {
    case OT_BINARY:
        return generateBinaryOpCode(opNode, block, table);
        break;
    case OT_READ:
        return generateReadOpCode(opNode, block, table);
        break;
    case OT_WRITE:
        return generateSetOpCode(opNode, block, table);
        break;
    case OT_CONST:
        return generateConstOpCode(opNode, block, table);
        break;
    case OT_CALL:
        return generateFunctionCall(opNode, block, table);
        break;
    }
}

