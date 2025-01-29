#include "ControlFlowCompiler.h"

void compile(CallGraphNode* callGraph) {
    initVM(&_virtualMachine);
    initSymbolTable(&_symbolTable);

}

void generateFunctionCode(ProgramUnit* unit) {
    printf("; Function: %s\n", unit->funcSignature->name);

    // Пролог функции: сохраняем контекст, если нужно
    printf("; Function prologue\n");

    // Загружаем аргументы функции в регистры или стек
    for (size_t i = 0; i < unit->funcSignature->FuncArgs->size; i++) {
        FuncArg* arg = (FuncArg*)getItem(unit->funcSignature->FuncArgs, i);
        int reg = allocateRegister();
        printf("LOAD R%d, %s ; Load argument %s\n", reg, arg->name, arg->name);
    }

    // Пример обработки графа потока управления (CFG)
    printf("; Control Flow Graph (CFG) processing for %s\n", unit->funcSignature->name);
    
    //generateCfgCode(unit->cfg);

    // Завершаем функцию инструкцией возврата
    printf("ret ; Return from function\n");

    // Освобождаем регистры после завершения работы функции
    while (usedRegisters > 0) {
        freeRegister();
    }
}

void generateFunctionCall(ProgramUnit* unit) {
    printf("; Call to function: %s\n", unit->funcSignature->name);

    // Генерация кода передачи аргументов
    for (size_t i = 0; i < unit->funcSignature->FuncArgs->size; i++) {
        FuncArg* arg = (FuncArg*)getItem(unit->funcSignature->FuncArgs, i);
        printf("push %s ; Push argument %s to stack\n", arg->name, arg->name);
    }

    // Вызов функции
    printf("call %s ; Call function %s\n", unit->funcSignature->name, unit->funcSignature->name);

    // Освобождение аргументов из стека после вызова
    for (size_t i = 0; i < unit->funcSignature->FuncArgs->size; i++) {
        printf("pop ; Clean up argument from stack\n");
    }
}

// Генерация кода для графа вызовов
void generateCallGraphCode(CallGraphNode* node) {
    if (node == NULL) return;

    // Генерируем код для текущей функции
    generateFunctionCode(node->unit);

    // Рекурсивно вызываем для детей
    for (size_t i = 0; i < node->children->size; i++) {
        CallGraphNode* child = getItem(node->children, i);
        generateFunctionCall(child->unit);
        generateCallGraphCode(child);
    }
}

void generateCfgCode(CfgNode* cfg) {

    if (cfg->opTree) {
        BasicBlock* block = createBasicBlock();

        generateOpTreeCode(cfg->opTree, block);
    }

    if (cfg->condJump) {
        generateCfgCode(cfg->condJump);
    }
    if (cfg->uncondJump)
    {
        generateCfgCode(cfg->uncondJump);
    }
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

int32_t generateLiteralOpCode(OpNode* opNode, BasicBlock* block) {
    int reg = allocateRegister();
    if (reg == -1) {  
        reg = 0;
        printf("PUSH R0 ; No registers available, saving R0\n");
        addInstruction(block, createInstruction(OC_PUSH, reg, 0, 0, 0, NULL));
    }
    freeRegister(reg);
}

int32_t generatePlaceOpCode(OpNode* opNode, BasicBlock* block) {
    int reg = allocateRegister();
    if (reg == -1) {
        reg = 0;
        printf("PUSH R0 ; No registers available, saving R0\n");
        addInstruction(block, createInstruction(OC_PUSH, reg, 0, 0, 0, NULL));
    }

    freeRegister(reg);

    return reg;
}

int32_t generateReadOpCode(OpNode* opNode, BasicBlock* block) {

    int reg = allocateRegister();
    if (reg == -1) {
        printf("PUSH R0 ; No registers available, saving R0\n");
        reg = 0;
        addInstruction(block, createInstruction(OC_PUSH, reg, 0, 0, 0, NULL));
    }

    printf("LOAD R%d, %s ; Read value from memory\n", reg, opNode->value);



    addInstruction(block, createInstruction(OC_LOAD, reg, 0, 0, , NULL));
    return reg; // Возвращаем регистр с загруженным значением
}

int32_t generateSetOpCode(OpNode* opNode, BasicBlock* block) {
    const L_VALUE_INDEX = 0;
    const R_VALUE_INDEX = 1;

    int32_t address = generateOpTreeCode(getItem(opNode->args, L_VALUE_INDEX), block);
    uint8_t reg = generateOpTreeCode(getItem(opNode->args, R_VALUE_INDEX), block);
    printf("STORE %d, R%d ; Store value into variable %s\n", address, reg, reg);
    
    addInstruction(block, createInstruction(OC_STORE, 0, reg, 0, address, NULL));
    freeRegister(reg);
}

int32_t generateOpTreeCode(OpNode* opNode, BasicBlock* block) {
    switch (opNode->opType) {
    case OT_BINARY:
        generateBinaryOpCode(opNode, block);
        break;
    case OT_LITERAL:
        generateLiteralOpCode(opNode, block);
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
    }
}

