#include "ControlFlowCompiler.h"

Array* compile(CallGraphNode* callGraph, SymbolTable* globalSymbolTable) {
    Array* modules = buildArray(sizeof(Module), COUNT_MODULES);

    MarkGenerator* markGen = createMarkGen();

    traverseCallGraph(callGraph, modules, markGen, globalSymbolTable);

    // free всех структур

    return modules;
}

void traverseCallGraph(CallGraphNode* root, Array* modules, MarkGenerator* markGen, SymbolTable* globalTable) {
    if (root == NULL) return;

    Module* generateModule = NULL;
    MachineState* state = createMachineState(markGen);

    // Генерируем код для текущей функции
    if (!root->unit->isBuiltIn) {
        generateModule = generateFunctionCode(root->unit, state, globalTable);
        if(generateModule)
            pushBack(modules, generateModule);
    }
   
    // Рекурсивно вызываем для детей
    for (size_t i = 0; i < root->children->size; i++) {
        CallGraphNode* child = getItem(root->children, i);
        traverseCallGraph(child, modules, markGen, globalTable);
    }
}

void prepareMachineState(MachineState* state, ProgramUnit* unit, Array* prologue) {
    state->offset = 0;
    state->offset += VARIABLE_SIZE; // ret
    state->offset += (unit->funcSignature->funcArgs->size * VARIABLE_SIZE); // выделяем место под аргументы
    I_PUSH(bp, prologue);
    I_MOV(bp, sp, prologue);
}

void bfsToPrepare(CfgNode* start, MachineState* state) {

    if (!start) return;

    bool visited[MAX_NODES] = { false };
    Queue q;
    initQueue(&q);

    enqueue(&q, start);
    visited[start->id] = true;

    while (!isEmpty(&q)) {
        CfgNode* current = dequeue(&q);

        state->cfgNodeMarks[current->id] = getConditionMark(state->markGen);

        if (current->uncondJump && !visited[current->uncondJump->id]) {
            enqueue(&q, current->uncondJump);
            visited[current->uncondJump->id] = true;
        }

        if (current->condJump && !visited[current->condJump->id]) {
            enqueue(&q, current->condJump);
            visited[current->condJump->id] = true;
        }
    }
}

Module* generateFunctionCode(ProgramUnit* unit, MachineState* state, SymbolTable* globalTable) {
    Module* generateModule = createModule(unit);

    if (findSymbol(globalTable, unit->funcSignature->name))
    {
        // если данную функцию уже скомпилировали то просто пропускаем её

        return NULL;
    }

    addSymbol(globalTable, unit->funcSignature->name, SYMBOL_FUNCTION, 0, unit->funcSignature->returnType);

    state->offset = -VARIABLE_SIZE;

    // Пролог функции: сохраняем контекст
    if (strcmp(unit->funcSignature->name, "main") != 0)
        prepareMachineState(state, unit, generateModule->prologue);

    if (unit->funcSignature->funcArgs->size > 0) {
        for (int i = unit->funcSignature->funcArgs->size - 1; i >= 0; i--) {

            FuncArg* arg = (FuncArg*)getItem(unit->funcSignature->funcArgs, i);
            Symbol* argSymbol = findSymbol(unit->currentTable, arg->name);

            if (argSymbol) {
                argSymbol->location = LOC_STACK;
                argSymbol->address = state->offset;
                createVariable(state, arg->name, arg->type);
            }
            else {
                // обработка ошибки
                collectError(ST_COMPILE, "func arg not found", arg->name, unit->ast->line);
            }
        }

        state->offset -= 4; // ret
        state->offset -= 4; // bp
    }
    
    size_t localOffset = 0;

    for (size_t i = 0; i < unit->currentTable->size; i++) {
        Symbol symbol = unit->currentTable->symbols[i];
        if (symbol.type == SYMBOL_VARIABLE) {
            Symbol symbol = unit->currentTable->symbols[i];
            symbol.location = LOC_STACK;
            symbol.address = state->offset;
            createVariable(state, symbol.name, symbol.type);
            localOffset += 4;
        }
    }
    I_SUBI(sp, sp, localOffset, generateModule->prologue);

    reg returnReg = -1;

    bfsToPrepare(unit->cfg, state);

    returnReg = bfsCfg(unit->cfg, generateModule, state, returnReg);
    
    if (unit->funcSignature->returnType != TYPE_VOID)
        I_MOV(tmp, returnReg, generateModule->epilogue);

    if (strcmp(unit->funcSignature->name, "main") != 0) {
        I_MOV(sp, bp, generateModule->epilogue);
        I_POP(bp, generateModule->epilogue);
    }

    if (strcmp(unit->funcSignature->name, "main") == 0)
        I_HALT(generateModule->epilogue);
    else
        I_RET(generateModule->epilogue);

    return generateModule;
}

CfgNode* resolveEmptyBlock(CfgNode* node) {
    while (node && !node->opTree && !node->condJump)
        node = node->uncondJump;
    return node;
}


reg bfsCfg(CfgNode* start, Module* genModule, MachineState* state, reg returnReg) {
    if (!start) return;

    bool visited[MAX_NODES] = { false };
    Queue q;
    initQueue(&q);

    enqueue(&q, start);
    visited[start->id] = true;

    while (!isEmpty(&q)) {
        CfgNode* current = dequeue(&q);

        int uncondJumpId = -1;
        int condJumpId = -1;

        if (current->uncondJump) {
            CfgNode* node = resolveEmptyBlock(current->uncondJump);
            if (node) uncondJumpId = node->id;
        }

        if (current->condJump) {
            CfgNode* node = resolveEmptyBlock(current->condJump);
            if (node) condJumpId = node->id;
        }

        ExprContext* ctx = createExprContext(current->label, condJumpId);

        if (state->cfgNodeMarks[current->id]) {
            I_MARK(ctx->instructions, state->cfgNodeMarks[current->id]);
        }

        if (current->opTree) {
            ctx->state = state;
            free(ctx->state->allocator);
            ctx->state->allocator = createRegAllocator();

            returnReg = generateOpTreeCode(current->opTree, ctx);

            if (uncondJumpId >= 0 && state->cfgNodeMarks[uncondJumpId])
                I_JMP(state->cfgNodeMarks[uncondJumpId], ctx->instructions);
        }
        
        pushBack(genModule->exprContextList, ctx);

        if (current->uncondJump && !visited[current->uncondJump->id]) {
            enqueue(&q, current->uncondJump);
            visited[current->uncondJump->id] = true;
        }

        if (current->condJump && !visited[current->condJump->id]) {
            enqueue(&q, current->condJump);
            visited[current->condJump->id] = true;
        }

    }
    return returnReg;
}

reg generateFunctionCall(OpNode* opNode, ExprContext* ctx) {
    const int FUNC_NAME_INDEX = 0;

    OpNode* funcNameOpNode = getItem(opNode->args, FUNC_NAME_INDEX);

    // Генерация кода передачи аргументов
    for (size_t i = opNode->args->size - 1; i > FUNC_NAME_INDEX; i--) {
        OpNode* funcArg = getItem(opNode->args, i);
        reg reg = generateOpTreeCode(funcArg, ctx);
        I_PUSH(reg, ctx->instructions);
    }

    // Вызов функции
    I_CALL(funcNameOpNode->value, ctx->instructions);

    return tmp;
}

reg generateBinaryOpCode(OpNode* opNode, ExprContext* ctx) {
    OpNode* left = getItem(opNode->args, 0);
    OpNode* right = getItem(opNode->args, 1);

    reg regDest = allocateRegister(ctx->state->allocator);

    reg regSrc1 = generateOpTreeCode(left, ctx);
    reg regSrc2 = generateOpTreeCode(right, ctx);

    if (!strcmp(opNode->value, "+")) {
        I_ADD(regDest, regSrc1, regSrc2, ctx->instructions);
    }
    else if (!strcmp(opNode->value, "-")) {
        I_SUB(regDest, regSrc1, regSrc2, ctx->instructions);
    }
    else if (!strcmp(opNode->value, "*")) {
        I_MUL(regDest, regSrc1, regSrc2, ctx->instructions);
    }
    else if (!strcmp(opNode->value, "/")) {
        I_DIV(regDest, regSrc1, regSrc2, ctx->instructions);
    }
    else if (!strcmp(opNode->value, "==")) {
        I_CMP(regSrc1, regSrc2, ctx->instructions);
        char* condMark = ctx->state->cfgNodeMarks[ctx->condId];
        I_JE(condMark, ctx->instructions);
    }
    else if (!strcmp(opNode->value, "!=")) {
        I_CMP(regSrc1, regSrc2, ctx->instructions);
        char* condMark = ctx->state->cfgNodeMarks[ctx->condId];
        I_JNE(condMark, ctx->instructions);
    }
    else if (!strcmp(opNode->value, "<")) {
        I_CMP(regSrc1, regSrc2, ctx->instructions);
        char* condMark = ctx->state->cfgNodeMarks[ctx->condId];
        I_JL(condMark, ctx->instructions);
    }
    else if (!strcmp(opNode->value, ">")) {
        I_CMP(regSrc1, regSrc2, ctx->instructions);
        char* condMark = ctx->state->cfgNodeMarks[ctx->condId];
        I_JG(condMark, ctx->instructions);
    }
    else if (!strcmp(opNode->value, "<=")) {
        I_CMP(regSrc1, regSrc2, ctx->instructions);
        char* condMark = ctx->state->cfgNodeMarks[ctx->condId];
        I_JLE(condMark, ctx->instructions);
    }
    else if (!strcmp(opNode->value, ">=")) {
        I_CMP(regSrc1, regSrc2, ctx->instructions);
        char* condMark = ctx->state->cfgNodeMarks[ctx->condId];
        I_JGE(condMark, ctx->instructions);
    }
    else {
        // неизвестная бинарная операция
        collectError(ST_COMPILE, "unknown binary operation", opNode->value, opNode->ast->line);
    }

    //freeRegister(&_virtualMachine.allocator, regSrc1);
    //freeRegister(&_virtualMachine.allocator, regSrc2);
    return regDest;
}

reg generateReadOpCode(OpNode* opNode, ExprContext* ctx) {

    OpNode* arg = getItem(opNode->args, 0);

    reg dest = allocateRegister(ctx->state->allocator);

    Variable* var = findVariable(arg->value, ctx->state);
    if (var) {
        if (var->address >= 0)
            I_LOAD_OFF_PLUS(dest, bp, var->address, ctx->instructions);
        else
            I_LOAD_OFF_MINUS(dest, bp, var->address, ctx->instructions);
    }
    else {
        // обработка ошибки
        collectError(ST_COMPILE, "Var can not find", arg->value, opNode->ast->line);
    }

    return dest; // Возвращаем регистр с загруженным значением
}

reg generateSetOpCode(OpNode* opNode, ExprContext* ctx) {
    const int L_VALUE_INDEX = 0;
    const int R_VALUE_INDEX = 1;

    OpNode* lValueOpNode = getItem(opNode->args, L_VALUE_INDEX);
    OpNode* rValueOpNode = getItem(opNode->args, R_VALUE_INDEX);

    reg rValueReg = generateOpTreeCode(rValueOpNode, ctx);

    if (lValueOpNode->opType == OT_PLACE) {
        Variable* var = findVariable(lValueOpNode->value, ctx->state);

        if (var) {
            int32_t reg3 = allocateRegister(ctx->state->allocator);

            if (reg3 >= 0) {
                if (var->address >= 0)
                    I_STORE_OFF_PLUS(rValueReg, bp, var->address, ctx->instructions);
                else
                    I_STORE_OFF_MINUS(rValueReg, bp, var->address, ctx->instructions);
            }
            else {
                // недостаточно регистров
                collectError(ST_COMPILE, "failed to alloc", "reg", opNode->ast->line);
            }
        }
        else {
            // обработка ошибки
            collectError(ST_COMPILE, "Variable can not find", lValueOpNode->value, opNode->ast->line);
        }
    }
    else {

    }
    return rValueReg;
}

reg generateConstOpCode(OpNode* opNode, ExprContext* ctx) {
    const int INDEX_CONST_VALUE = 0;
    OpNode* constValueOpNode = getItem(opNode->args, INDEX_CONST_VALUE);

    reg reg = allocateRegister(ctx->state->allocator);

    if (opNode->valueType == TYPE_INT) {
        I_MOVI(reg, constValueOpNode->value, ctx->instructions);
    }
    else if(opNode->valueType == TYPE_STRING) {
        bool isRepeat = false;
        char* mark = getStringMark(ctx->state->markGen, constValueOpNode->value, &isRepeat);
        if (!isRepeat) {
            I_CONST_STR(mark, constValueOpNode->value, ctx->constDataList);
        }
        I_MOVI(reg, mark, ctx->instructions);
    }

    return reg;
}

reg generateOpTreeCode(OpNode* opNode, ExprContext* ctx) {
    switch (opNode->opType) {
    case OT_BINARY: return generateBinaryOpCode(opNode, ctx);
        break;
    case OT_READ: return generateReadOpCode(opNode, ctx);
        break;
    case OT_WRITE: return generateSetOpCode(opNode, ctx);
        break;
    case OT_CONST: return generateConstOpCode(opNode, ctx);
        break;
    case OT_CALL: return generateFunctionCall(opNode, ctx);
        break;
    case OT_INDEX: return generateIndexOpCode(opNode, ctx);
        break;
    }
}

reg generateIndexOpCode(OpNode* opNode, ExprContext* ctx)
{
    OpNode* ptrNameOp = getItem(opNode->args, 0);
    OpNode* indexOp = getItem(opNode->args, 1);

    reg ptr = generateBinaryOpCode(ptrNameOp, ctx);
    reg index = generateBinaryOpCode(indexOp, ctx);

    reg dest = allocateRegister(ctx->state->allocator);
    Variable* var = findVariable(ptrNameOp->value, ctx->state);
    getTypeSize(var->type);
    switch ()
    {
    default:
        break;
    }

    return dest;
}

