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
        int reg = allocateRegister();
        addInstruction(prologueBlock, createInstruction(OC_POP, reg, 0, 0, NULL, NULL));
    }

    //помещаем блок пролога в массив
    blocks[counter_blocks] = prologueBlock;
    counter_blocks++;
    
    traverseCfg(unit->cfg);

    // Завершаем функцию инструкцией возврата
    BasicBlock* finalBlock = createBasicBlock();
    addInstruction(finalBlock, createInstruction(OC_RET, 0, 0, 0, NULL, NULL));
   
    blocks[counter_blocks] = finalBlock;
    counter_blocks++;

    // Освобождаем регистры после завершения работы функции
    while (usedRegisters > 0) {
        freeRegister();
    }
}

BasicBlock* traverseCfg(CfgNode* cfg) {
    BasicBlock* block = NULL;
    if (cfg->opTree) {
        block = createBasicBlock();

        generateOpTreeCode(cfg->opTree, block);

        blocks[counter_blocks] = block;
        counter_blocks++;
    }

    if (cfg->condJump) {
        traverseCfg(cfg->condJump);
    }
    if (cfg->uncondJump) {
        traverseCfg(cfg->uncondJump);
    }
    if (block)
        return block;
    else return NULL;
}

void generateFunctionCall(OpNode* opNode, BasicBlock* block) {
    const int FUNC_NAME_INDEX = 0;

    StackFrame frame = peekFrame(&_callStack);

    ProgramUnit* unit = frame.programUnit;

    Array* funcArgs = unit->funcSignature->funcArgs;

    // Генерация кода передачи аргументов
    for (size_t i = FUNC_NAME_INDEX + 1; i < funcArgs->size; i++) {
        FuncArg* funcArg = (FuncArg*)getItem(funcArgs, i);
        Symbol* symbol = findSymbol(&_symbolTable, funcArg->name);
        addInstruction(block, createInstruction(OC_PUSH, 0, 0, 0, symbol->address, NULL));
    }

    // Вызов функции
    addInstruction(block, createInstruction(OC_CALL, 0, 0, 0, unit->funcSignature->name, NULL));
}

void generateBinaryOpCode(OpNode* opNode, BasicBlock* block) {
    OpNode* left = getItem(opNode->args, 0);
    OpNode* right = getItem(opNode->args, 1);

    int destReg = allocateRegister();

    int reg1 = allocateRegister();
    if (reg1 == -1) {
        //printf("PUSH R0 ; No registers available, saving R0\n");
        addInstruction(block, createInstruction(OC_PUSH, reg1, 0, 0, NULL, NULL));
        reg1 = 0;
    }
    generateOpTreeCode(left, block);
    printf("MOV R%d, R0 ; Save left operand\n", reg1);

    int reg2 = allocateRegister();
    if (reg2 == -1) {
        //printf("PUSH R0 ; No registers available, saving R0\n");
        addInstruction(block, createInstruction(OC_PUSH, reg2, 0, 0, NULL, NULL));
        reg2 = 0;
    }
    generateOpTreeCode(right, block);

    if (!strcmp(opNode->value, "+")) {
        addInstruction(block, createInstruction(OC_ADD, destReg, reg1, reg2, NULL, NULL));
    }
    else if (!strcmp(opNode->value, "-")) {
        addInstruction(block, createInstruction(OC_SUB, destReg, reg1, reg2, NULL, NULL));
    }
    else {
        // неизвестная бинарная операция
    }

    freeRegister(reg1);
    freeRegister(reg2);
}

int32_t generatePlaceOpCode(OpNode* opNode, BasicBlock* block) {
    Symbol* find = findSymbol(&_symbolTable, opNode->value);

    if (find) {
        return find->address;
    }

    size_t typeSize = getTypeSize(opNode->valueType);
    char relativeAddress[16];

    _itoa_s(typeSize, relativeAddress, 16, 10);
    addInstruction(block, createInstruction(OC_SUB, sp, sp, 0, relativeAddress, NULL));
    addSymbol(&_symbolTable, opNode->value, SYMBOL_VARIABLE, 0, typeSize, opNode->valueType);

    return typeSize;
}

int32_t generateReadOpCode(OpNode* opNode, BasicBlock* block) {

    int reg = allocateRegister();
    if (reg == -1) {
        reg = 0;
        addInstruction(block, createInstruction(OC_PUSH, reg, 0, 0, NULL, NULL));
    }

    OpNode* arg = getItem(opNode->args, 0);

    StackFrame frame = peekFrame(&_callStack);

    Symbol* symbol = findSymbol(&_symbolTable, arg->value);

    if (!symbol) {
        // обработка ошибки - переменная не инициализирована

        return;
    }

    addInstruction(block, createInstruction(OC_LOAD, reg, 0, 0, symbol->address, NULL));
    return reg; // Возвращаем регистр с загруженным значением
}

int32_t generateSetOpCode(OpNode* opNode, BasicBlock* block) {
    const int L_VALUE_INDEX = 0;
    const int R_VALUE_INDEX = 1;

    OpNode* lValueOpNode = getItem(opNode->args, L_VALUE_INDEX);

    char valuePlaceAddress[16];

    if (lValueOpNode->opType == OT_PLACE) {
        _itoa_s(generatePlaceOpCode(lValueOpNode, block), valuePlaceAddress, 16, 10);
    }
    else {

    }

    OpNode* rValueOpNode = getItem(opNode->args, R_VALUE_INDEX);

    int32_t reg2 = generateOpTreeCode(rValueOpNode, block);


    addInstruction(block, createInstruction(OC_MOVI, 0, reg2, 0, valuePlaceAddress, NULL));

    freeRegister(reg2);
   
}

int32_t generateConstOpCode(OpNode* opNode, BasicBlock* block) {
    const int INDEX_CONST_VALUE = 0;
    OpNode* constValueOpNode = getItem(opNode->args, INDEX_CONST_VALUE);

    int32_t reg = allocateRegister();
    addInstruction(block, createInstruction(OC_LOAD, reg, 0, 0, constValueOpNode->value, NULL));

    return reg;
}

int32_t generateOpTreeCode(OpNode* opNode, BasicBlock* block) {
    switch (opNode->opType) {
    case OT_BINARY:
        generateBinaryOpCode(opNode, block);
        break;
    case OT_READ:
        return generateReadOpCode(opNode, block);
        break;
    case OT_WRITE:
        return generateSetOpCode(opNode, block);
        break;
    case OT_CONST:
        return generateConstOpCode(opNode, block);
        break;
    case OT_CALL:
        generateFunctionCall(opNode, block);
        break;
    }
}

