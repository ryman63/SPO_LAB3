#include "ControlFlowCompiler.h"

void compile(CallGraphNode* callGraph) {
    initVM(&_virtualMachine);
    initSymbolTable(&_symbolTable);
    initCallStack(&_callStack);

    traverseCallGraph(callGraph);

    size_t countInstr = calculateCountInstr();

    Instruction* programLinearCode = malloc(sizeof(Instruction) * countInstr);

    loadProgram(programLinearCode, blocks, counter_blocks);

    parseInstructionInLinearCode(&_virtualMachine, programLinearCode);

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
    // создаём блок для пролога функции
    BasicBlock* prologueBlock = createBasicBlock();

    // создание метки
    addInstruction(prologueBlock, createInstruction(OC_MARK, 0, 0, 0, NULL, unit->funcSignature->name));

    // Пролог функции: сохраняем контекст
    // Загружаем аргументы функции в регистры или стек
    for (size_t i = 0; i < unit->funcSignature->FuncArgs->size; i++) {
        
        FuncArg* arg = (FuncArg*)getItem(unit->funcSignature->FuncArgs, i);
        int reg = allocateRegister();
        addInstruction(prologueBlock, createInstruction(OC_LOAD, reg, 0, 0, NULL, NULL));
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

void generateFunctionCall(ProgramUnit* unit) {
    BasicBlock* callBlock = createBasicBlock();

    // Генерация кода передачи аргументов
    for (size_t i = 0; i < unit->funcSignature->FuncArgs->size; i++) {
        FuncArg* arg = (FuncArg*)getItem(unit->funcSignature->FuncArgs, i);
        Symbol* symbol = findSymbol(&_symbolTable, arg->name);
        addInstruction(callBlock, createInstruction(OC_PUSH, 0, 0, 0, symbol->address, NULL));
    }

    // Вызов функции
    addInstruction(callBlock, createInstruction(OC_CALL, 0, 0, 0, unit->funcSignature->name, NULL));



    //// Освобождение аргументов из стека после вызова
    //for (size_t i = 0; i < unit->funcSignature->FuncArgs->size; i++) {
    //    addInstruction(callBlock, createInstruction(OC_POP, ))
    //}
}

BasicBlock* traverseCfg(CfgNode* cfg) {
    BasicBlock* block = NULL;
    if (cfg->opTree) {
        block = createBasicBlock();

        generateOpTreeCode(cfg->opTree, block);
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

void generateBinaryOpCode(OpNode* opNode, BasicBlock* block) {
    OpNode* left = getItem(opNode->args, 0);
    OpNode* right = getItem(opNode->args, 1);

    int reg1 = allocateRegister();
    if (reg1 == -1) {
        printf("PUSH R0 ; No registers available, saving R0\n");
        reg1 = 0;
    }
    generateOpTreeCode(left, block);
    printf("MOV R%d, R0 ; Save left operand\n", reg1);

    int reg2 = allocateRegister();
    if (reg2 == -1) {
        printf("PUSH R0 ; No registers available, saving R0\n");
        reg2 = 0;
    }
    generateOpTreeCode(right, block);

    /*switch (opNode->op) {
    case OP_ADD:
        printf("ADD R0, R%d, R%d ; R0 = R%d + R%d\n", reg1, reg2, reg1, reg2);
        break;
    case OP_SUB:
        printf("SUB R0, R%d, R%d ; R0 = R%d - R%d\n", reg1, reg2, reg1, reg2);
        break;
    case OP_MUL:
        printf("MUL R0, R%d, R%d ; R0 = R%d * R%d\n", reg1, reg2, reg1, reg2);
        break;
    case OP_DIV:
        printf("DIV R0, R%d, R%d ; R0 = R%d / R%d\n", reg1, reg2, reg1, reg2);
        break;
    }*/

    freeRegister(reg1);
    freeRegister(reg2);
}

char* getLiteral(OpNode* opNode) {
    /*int reg = allocateRegister();
    if (reg == -1) {  
        reg = 0;
        printf("PUSH R0 ; No registers available, saving R0\n");
        addInstruction(block, createInstruction(OC_PUSH, reg, 0, 0, 0, NULL));
    }
    freeRegister(reg);*/
    return strCpy(opNode->value);
}

int32_t generatePlaceOpCode(OpNode* opNode, BasicBlock* block) {
    int reg = allocateRegister();
    if (reg == -1) {
        reg = 0;
        printf("PUSH R0 ; No registers available, saving R0\n");
        addInstruction(block, createInstruction(OC_PUSH, reg, 0, 0, NULL, NULL));
    }

    freeRegister(reg);

    return reg;
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

    addInstruction(block, createInstruction(OC_LOAD, reg, 0, 0, symbol->address, NULL));
    return reg; // Возвращаем регистр с загруженным значением
}

int32_t generateSetOpCode(OpNode* opNode, BasicBlock* block) {
    const L_VALUE_INDEX = 0;
    const R_VALUE_INDEX = 1;

    
    uint8_t reg = generateOpTreeCode(getItem(opNode->args, R_VALUE_INDEX), block);
    ////printf("STORE %d, R%d ; Store value into variable %s\n", address, reg, reg);
    //
    //addInstruction(block, createInstruction(OC_STORE, 0, reg, 0, address, NULL));
    freeRegister(reg);
}

int32_t generateOpTreeCode(OpNode* opNode, BasicBlock* block) {
    switch (opNode->opType) {
    case OT_BINARY:
        generateBinaryOpCode(opNode, block);
        break;
    case OT_PLACE:
        generatePlaceOpCode(opNode, block);
        break;
    case OT_READ:
        generateReadOpCode(opNode, block);
        break;
    case OT_SET:
        generateSetOpCode(opNode, block);
        break;
    case OT_CALL: {
            OpNode* arg = getItem(opNode->args, 0);
            Symbol* symbol = findSymbol(&_symbolTable, arg->value);
            generateFunctionCall(symbol->name);
        }
        break;
    }
}

