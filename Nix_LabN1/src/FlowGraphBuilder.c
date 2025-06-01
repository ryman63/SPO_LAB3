#include "FlowGraphBuilder.h"

void registerAllBuiltInFuncs() {
	REGISTER_BUILTIN("print",
		"iostream.asm",
		createBuiltInType(TYPE_VOID, NULL),
		FUNC_ARGS(
			FUNC_ARG("str", createBuiltInPtr(TYPE_CHAR, NULL))
		)
	);
	REGISTER_BUILTIN("input",
		"iostream.asm",
		createBuiltInType(TYPE_VOID, NULL),
		FUNC_ARGS(
			FUNC_ARG("str", createBuiltInArray(TYPE_CHAR, STRING_CAPACITY, NULL))
		)
	);
	REGISTER_BUILTIN("atoi",
		"convert.asm",
		createBuiltInType(TYPE_INT, NULL),
		FUNC_ARGS(
			FUNC_ARG("str", createBuiltInPtr(TYPE_CHAR, NULL))
		)
	);
	REGISTER_BUILTIN("itoa",
		"convert.asm",
		createBuiltInType(TYPE_VOID, NULL),
		FUNC_ARGS(
			FUNC_ARG("src", createBuiltInType(TYPE_INT, NULL)),
			FUNC_ARG("buffer", createBuiltInPtr(TYPE_CHAR, NULL))
		)
	);
}

void defineAllBuiltInFuncs(Array* programUnitStorage) {
	registerAllBuiltInFuncs();
	
	size_t countBuiltInFuncs = getBuiltInFuncCount();
	for (size_t i = 0; i < countBuiltInFuncs; i++) {
		BuiltInFunc srcFunc = getBuiltInFunc(i);

		Array* funcArgsArray = buildArray(sizeof(FuncArg), BUILTIN_MAX_COUNT_ARGS);
		for (size_t j = 0; j < srcFunc.countArgs; j++) {
			FuncArg* arg = malloc(sizeof(FuncArg));
			arg->name = srcFunc.args[j].name;
			arg->type = srcFunc.args[j].type;
			pushBack(funcArgsArray, &arg);
		}

		ProgramUnit builtInProgramUnit = createBuiltInProgramUnit(funcArgsArray, srcFunc.name, srcFunc.returnType, srcFunc.sourceFileName);

		pushBack(programUnitStorage, &builtInProgramUnit);
	}
}

CallGraphNode* analysis(Array* srcFiles, char* outputDir, Array* astList) {

	for (size_t i = 0; i < srcFiles->size; i++) {
		char* buffErrors = malloc(sizeof(char) * 1024);
		buffErrors[0] = '\0';

		SourceFile* srcFile = getItem(srcFiles, i);

		AstNode* rootAst = parseCustomLang(getFilePath(srcFile), buffErrors);

		pushBack(astList, rootAst);

		int generateResult = generateDGML(rootAst, "ast.dgml");

		if (generateResult >= 0) printf("DGML file succesfully generated\n");
		else printf("DGML file generation error\n");

		//printAst(root, 0);

		Array* funcNodes = buildArray(sizeof(AstNode), 5);
		dfs(rootAst, funcNodes, "FUNC_DEF");

		if (funcNodes->size == 0) {
			return;
		}
		
		programUnitStorage = buildArray(sizeof(ProgramUnit), 4);

		// BuiltIn функции
		defineAllBuiltInFuncs(programUnitStorage);

		// пользовательские функции
		for (size_t i = 0; i < funcNodes->size; i++) {
			AstNode* funcNode = getItem(funcNodes, i);
			ProgramUnit programUnit;

			programUnit.isBuiltIn = false;
			programUnit.sourceFile = srcFile;
			programUnit.funcSignature = buildFuncSignature(funcNode);

			pushBack(programUnitStorage, &programUnit);

			//freeSymbolTable(funcSymbolTable);
		}

		size_t counter = 0;

		for (size_t i = 0; i < programUnitStorage->size; i++) {
			SymbolTable* funcSymbolTable = malloc(sizeof(SymbolTable));
			initSymbolTable(funcSymbolTable);
			currentTable = funcSymbolTable;

			ProgramUnit* programUnit = getItem(programUnitStorage, i);

			if (programUnit->isBuiltIn) continue;

			AstNode* funcNode = getItem(funcNodes, counter);

			programUnit->cfg = handleFunctionBody(funcNode, programUnit->funcSignature->funcArgs);
			programUnit->currentTable = funcSymbolTable;
			programUnit->ast = funcNode;

			char* buff = malloc(sizeof(char) * 8);
			itoa(i, buff, 10);
			generateCfgDGML(programUnit->cfg, strcat(buff, ".dgml"));
			TraverseCfg(programUnit->cfg, generateOpTreeDGML);
			counter++;
		}

		CallGraphNode* callGraph = buildCallGraph(programUnitStorage);

		generateCallGraphDGML(callGraph, outputDir);

		if (strlen(buffErrors) > 0)
			printf(buffErrors);


		freeArray(&programUnitStorage);
		freeArray(&funcNodes);
		free(buffErrors);

		return callGraph;
	}
}

ValueType* getType(AstNode* rootTypeRefAst) {
	AstNode* typeOfType = getItem(rootTypeRefAst->children, 0);
	TypeKind kind = getTypeKindFromString(typeOfType->token);
	switch (kind)
	{
	case TYPE_KIND_BUILTIN: {
		AstNode* builtInType = getItem(typeOfType->children, 0);
		BuiltInType type = getBuiltInTypeFromString(builtInType->token);
		return createBuiltInType(type, rootTypeRefAst);
	} break;
	case TYPE_KIND_ARRAY: {
		AstNode* typeOfArray = getItem(typeOfType->children, 0);
		BuiltInType type = getBuiltInTypeFromString(typeOfArray->token);
		AstNode* sizeOfArray = NULL;
		if (typeOfType->children->size > 1)
			sizeOfArray = getItem(typeOfType->children, 1);

		if (sizeOfArray) {
			int size = atoi(sizeOfArray->token);
			return createBuiltInArray(type, size, rootTypeRefAst);
		}
		return createBuiltInArray(type, 0, rootTypeRefAst);
	}
	/*case TYPE_KIND_POINTER: {
		
	} break;*/
	default: {
		collectError(ST_TYPING, "kind of type unsupported", rootTypeRefAst->token, rootTypeRefAst->line);
		
	} break;
	}

	return NULL;
}

FuncSignature* buildFuncSignature(AstNode* rootFuncAst) {
	FuncSignature* signature = createFuncSignature();
	AstNode* funcSignNode = getItem(rootFuncAst->children, 0);
	AstNode* NameNode = getItem(funcSignNode->children, 0);

	signature->name = strCpy(NameNode->token);

	AstNode* ListArgsNode = getItem(funcSignNode->children, 1);
	if (ListArgsNode) {
		Array* args = ListArgsNode->children;
		if (args) {
			for (size_t i = 0; i < args->size; i++) {
				FuncArg* funcArg = (FuncArg*)malloc(sizeof(FuncArg));
				AstNode* argNode = getItem(args, i);
				Array* argChildren = argNode->children;

				AstNode* ArgNameNode = getItem(argChildren, 0);
				funcArg->name = strCpy(ArgNameNode->token);
				
				AstNode* argType = getItem(argChildren, 1);
				ValueType* type = getType(argType);
				funcArg->type = type;

				pushBack(signature->funcArgs, funcArg);
			}
		}
	}

	if (funcSignNode->children->size > 2) {
		AstNode* returnTypeNode = getItem(funcSignNode->children, 2);
		signature->returnType = getType(returnTypeNode);
	}
	else
		signature->returnType = createBuiltInType(TYPE_VOID, rootFuncAst);
	

	return signature;
}

CfgNode* handleFunctionBody(AstNode* functionBodyAst, Array* funcArgs) {
	breakTargets = buildArray(sizeof(CfgNode), 4);

	if (!functionBodyAst)
		return NULL;

	// заполняем таблицу переменными из параметров функции
	for (size_t i = 0; i < funcArgs->size; i++) {
		FuncArg* arg = getItem(funcArgs, i);
		addSymbol(currentTable, arg->name, SYMBOL_FUNC_PARAM, -1, arg->type);
	}

	CfgNode* entryNode = createCfgNode("function entry", functionBodyAst);
	CfgNode* exitNode = createCfgNode("function exit", NULL);

	for (size_t i = 1; i < functionBodyAst->children->size; i++) {
		AstNode* statement = getItem(functionBodyAst->children, i);
		handleStatement(statement, entryNode, exitNode);
	}

	freeArray(&breakTargets);

	return entryNode;
}


CfgNode* handleStatement(AstNode* rootStatementAst, CfgNode* entryNode, CfgNode* exitNode) {
	if (!rootStatementAst)
		return NULL;

	AstNode* typeOfStatement = getItem(rootStatementAst->children, 0);

	if (typeOfStatement == NULL) {
		// error

		return NULL;
	}

	if (!strcmp(typeOfStatement->token, "BLOCK_STATEMENT")) {
		return handleBlockStatement(typeOfStatement, entryNode, exitNode);
	}
	else if (!strcmp(typeOfStatement->token, "IF_STATEMENT")) {
		return handleConditionStatement(typeOfStatement, entryNode, exitNode);
	}
	else if (!strcmp(typeOfStatement->token, "LOOP_STATEMENT")) {
		return handleLoopStatement(typeOfStatement, entryNode, exitNode);
	}
	else if (!strcmp(typeOfStatement->token, "BREAK_STATEMENT")) {
		return handleBreakStatement(typeOfStatement, entryNode, exitNode);
	}
	else if (!strcmp(typeOfStatement->token, "REPEAT_STATEMENT")) {
		return handleRepeatStatement(typeOfStatement, entryNode, exitNode);
	}
	else if (!strcmp(typeOfStatement->token, "EXPRESSION_STATEMENT")) {
		return handleExpressionStatement(typeOfStatement, entryNode, exitNode);
	}
	else {
		// обработка ошибки
		collectError(ST_ANALYZE, "Unhandled statement", typeOfStatement->token, typeOfStatement->line);

		return NULL;
	}
}

CfgNode* handleBlockStatement(AstNode* statementNodeAst, CfgNode* entryNode, CfgNode* exitNode) {
	if (!statementNodeAst) {
		// обработка ошибки
		collectError(ST_ANALYZE, "statement is", "NULL", -1);

		return NULL;
	}
	CfgNode* currentNode = entryNode;
	for (size_t i = 0; i < statementNodeAst->children->size; i++) {
		AstNode* childStatement = getItem(statementNodeAst->children, i);
		currentNode = handleStatement(childStatement, currentNode, exitNode);
	}
	//currentNode->condJump = exitNode;

	return currentNode;
}

CfgNode* handleConditionStatement(AstNode* statementNodeAst, CfgNode* entryNode, CfgNode* exitNode) {
	Array* children = statementNodeAst->children;

	if (children->size < 2 || children->size > 3) {
		char sizeStr[4];
		_itoa_s(children->size, sizeStr, 4, 10);
		collectError(ST_ANALYZE, "condition ast children incorrect", sizeStr, statementNodeAst->line);
		return NULL;
	}

	AstNode* exprNode = getItem(children, 0);
	AstNode* uncondStatement = getItem(children, 1);
	AstNode* condStatement = (children->size == 3) ? getItem(children, 2) : NULL;

	CfgNode* condBlock = createCfgNode("condBlock", exprNode);
	condBlock->opTree = handleExpression(getItem(exprNode->children, 0));

	entryNode->uncondJump = condBlock;

	//CfgNode* thenNode = createCfgNode("thenBlock", condStatement);
	CfgNode* joinNode = createCfgNode("joinBlock", NULL);

	//CfgNode* afterThenNode = handleStatement(condStatement, thenNode, exitNode);
	CfgNode* thenNode = handleStatement(uncondStatement, condBlock, joinNode);
	//condBlock->condJump = thenNode;
	//afterThenNode->condJump = joinNode;
	thenNode->uncondJump = joinNode;

	if (condStatement) {
		//CfgNode* elseNode = createCfgNode("elseBlock", uncondStatement);
		//condBlock->uncondJump = elseNode;
		//CfgNode* afterElseNode = handleStatement(uncondStatement, elseNode, exitNode);
		CfgNode* elseNode = handleStatement(condStatement, condBlock, joinNode);
		condBlock->uncondJump = elseNode;
		condBlock->condJump = thenNode;
		elseNode->uncondJump = joinNode;
	}
	else {
		condBlock->condJump = thenNode;
		condBlock->uncondJump = joinNode;
	}

	joinNode->uncondJump = exitNode;

	return joinNode;
}

CfgNode* handleLoopStatement(AstNode* statementNodeAst, CfgNode* entryNode, CfgNode* functionExitNode) {
	// Создаем отдельный узел для выхода из цикла (не путать с выходом из функции)
	CfgNode* loopExitNode = createCfgNode("loop exit", NULL);

	CfgNode* condBlock = createCfgNode("loop condition", statementNodeAst);
	entryNode->uncondJump = condBlock;

	// Добавляем loopExitNode в breakTargets (а не functionExitNode)
	pushBack(breakTargets, loopExitNode);

	Array* children = statementNodeAst->children;
	if (children->size < 1) {
		char childrens[4];
		_itoa_s(children->size, childrens, 4, 10);
		collectError(ST_ANALYZE, "loop ast children count incorrect", childrens, statementNodeAst->line);
		popBack(breakTargets);
		return NULL;
	}

	AstNode* exprNode = getItem(children, 0);
	condBlock->opTree = handleExpression(getItem(exprNode->children, 0));

	// Условие цикла:
	// - если true: переходим в тело цикла
	// - если false: выходим из цикла (loopExitNode)
	CfgNode* bodyEntry = createCfgNode("loop body entry", NULL);
	condBlock->condJump = bodyEntry;   // true -> тело цикла
	condBlock->uncondJump = loopExitNode; // false -> выход из цикла

	// Обрабатываем тело цикла
	CfgNode* lastBodyNode = bodyEntry;
	for (size_t i = 1; i < children->size; i++) {
		AstNode* statement = getItem(children, i);
		lastBodyNode = handleStatement(statement, lastBodyNode, condBlock);
	}

	// Последний узел тела цикла должен вернуться к условию
	if (lastBodyNode) {
		lastBodyNode->uncondJump = condBlock;
	}

	popBack(breakTargets); // Убираем loopExitNode из breakTargets

	// Теперь loopExitNode должен вести на следующий код после цикла
	// (пока просто соединяем с functionExitNode, но это должно быть улучшено)
	loopExitNode->uncondJump = functionExitNode;

	return loopExitNode;
}

CfgNode* handleRepeatStatement(AstNode* typeOfStatement, CfgNode* entryNode, CfgNode* exitNode) {
	CfgNode* bodyBlock = createCfgNode("body block", typeOfStatement);
	entryNode->uncondJump = bodyBlock;

	CfgNode* afterBodyBlock = NULL;
	pushBack(breakTargets, exitNode);

	Array* children = typeOfStatement->children;
	AstNode* exprNode = getItem(children, 1);
	if (children->size > 1) {
		for (size_t i = 0; i < children->size - 1; i++) {
			AstNode* statement = getItem(children, i);
			afterBodyBlock = handleStatement(statement, bodyBlock, bodyBlock);
		}
	}
	else {
		// обработка ошибки
		char childrens[4];

		_itoa_s(children->size, childrens, 4, 10);

		collectError(ST_ANALYZE, "repeat ast children incorrect", childrens, typeOfStatement->line);

		return NULL;
	}

	CfgNode* condBlock = createCfgNode("repeat condition block", exprNode);
	condBlock->opTree = handleExpression(getItem(exprNode->children, 0));

	condBlock->condJump = bodyBlock;
	condBlock->uncondJump = exitNode;

	return exitNode;
}

CfgNode* handleBreakStatement(AstNode* statementNodeAst, CfgNode* entryNode, CfgNode* exitNode) {
	entryNode->ast = statementNodeAst;

	if (breakTargets->size == 0) {
		// обработка ошибки
		collectError(ST_ANALYZE, "not found target", "break statement", statementNodeAst->line);

		return NULL;
	}

	CfgNode* breakTarget = popBack(breakTargets);
	entryNode->uncondJump = breakTarget;

	breakTarget->uncondJump = exitNode;

	return createCfgNode("break dead node", NULL);
}

CfgNode* handleExpressionStatement(AstNode* statementNodeAst, CfgNode* entryNode, CfgNode* exitNode) {
	CfgNode* newNode = createCfgNode("expression statement", statementNodeAst);

	AstNode* expressionStatementAst = getItem(statementNodeAst->children, 0);
	newNode->opTree = handleExpression(getItem(expressionStatementAst->children, 0));
	entryNode->uncondJump = newNode;
	newNode->uncondJump = exitNode;

	return newNode;
}


OpNode* handleExpression(AstNode* exprNode) {

	char* firstToken = exprNode->token;

	if (!strcmp(firstToken, "=")) {
		return handleSet(exprNode);
	}
	else if (!strcmp(firstToken, "+")) {
		return handleBinaryOp(exprNode);
	}
	else if (!strcmp(firstToken, "-")) {
		return handleBinaryOp(exprNode);
	}
	else if (!strcmp(firstToken, ">")) {
		return handleBinaryOp(exprNode);
	}
	else if (!strcmp(firstToken, "<")) {
		return handleBinaryOp(exprNode);
	}
	else if (!strcmp(firstToken, "*")) {
		return handleBinaryOp(exprNode);
	}
	else if (!strcmp(firstToken, "/")) {
		return handleBinaryOp(exprNode);
	}
	else if (!strcmp(firstToken, "==")) {
		return handleBinaryOp(exprNode);
	}
	else if (!strcmp(firstToken, "!=")) {
		return handleBinaryOp(exprNode);
	}
	else if(!strcmp(firstToken, "LITERAL_EXPR")) {
		return handleLiteralOp(exprNode);
	}
	else if (!strcmp(firstToken, "CALL_EXPR")) {
		return handleCallOp(exprNode);
	}
	else if (!strcmp(firstToken, "SLICE_EXPR")) {
		return handleSliceOp(exprNode);
	}
	else if (!strcmp(firstToken, "IDENTIFIER")) {
		return handleVarOp(exprNode);
	}
	else {
		collectError(ST_ANALYZE, "unsupported operation", firstToken, exprNode->line);
	}
	
	return NULL;
}

OpNode* handleSet(AstNode* opNodeAst) {
	AstNode* lValueIdentifier = getItem(opNodeAst->children, 0);
	AstNode* lValue = getItem(lValueIdentifier->children, 0);
	AstNode* rValueIdentifier = getItem(opNodeAst->children, 1);

	OpNode* resultOp = createOpNode("set", OT_WRITE, opNodeAst);

	OpNode* lValueOp = NULL;
	if (!strcmp(lValue->token, "SLICE_EXPR"))
		lValueOp = handleSliceOp(lValue);
	else
		lValueOp = createOpNode(strCpy(lValue->token), OT_PLACE, opNodeAst);

	OpNode* rValueOp = handleExpression(rValueIdentifier);

	if(rValueOp)
		lValueOp->valueType = rValueOp->valueType;

	pushBack(resultOp->args, lValueOp);

	if(rValueOp)
		pushBack(resultOp->args, rValueOp);
	else {
		// обработка ошибки
		collectError(ST_ANALYZE, "is not found", rValueIdentifier->token, rValueIdentifier->line);

		return NULL;
	}

	if (!findSymbol(currentTable, lValueOp->value)) {

		addSymbol(currentTable, lValueOp->value, SYMBOL_VARIABLE, -1, lValueOp->valueType);
	}
		

	return resultOp;
}

OpNode* handleBinaryOp(AstNode* opNodeAst) {
	OpNode* resultOp = createOpNode(strCpy(opNodeAst->token), OT_BINARY, opNodeAst);

	AstNode* lValue = getItem(opNodeAst->children, 0);
	AstNode* rValue = getItem(opNodeAst->children, 1);

	OpNode* lValueOp = handleExpression(lValue);
	OpNode* rValueOp = handleExpression(rValue);
	
	resultOp->valueType = compareTypes(lValueOp->valueType, rValueOp->valueType, opNodeAst);

	if(lValueOp)
		pushBack(resultOp->args, lValueOp);
	if(rValueOp)
		pushBack(resultOp->args, rValueOp);

	return resultOp;
}

OpNode* handleLiteralOp(AstNode* exprNode) {
	AstNode* literalNodeType = getItem(exprNode->children, 0);
	AstNode* literalNode = getItem(literalNodeType->children, 0);

	OpNode* resultOp = createOpNode("const", OT_CONST, literalNode);
	OpNode* litOp = createOpNode(strCpy(literalNode->token), OT_LITERAL, literalNode);

	pushBack(resultOp->args, litOp);

	litOp->valueType = builtInTypeIdentify(literalNodeType);
	resultOp->valueType = litOp->valueType;

	return resultOp;
}

OpNode* handleVarOp(AstNode* exprNode) {
	AstNode* varAst = getItem(exprNode->children, 0);

	Symbol* varSymbol = findSymbol(currentTable, varAst->token);

	if (!varSymbol) {
		// обработка ошибки - переменная не инициализирована
		collectError(ST_ANALYZE, "unknown variable", varAst->token, varAst->line);

		return NULL;
	}

	OpNode* resultOp = createOpNode("read", OT_READ, varAst);

	OpNode* varOp = createOpNode(strCpy(varAst->token), OT_PLACE, varAst);

	varOp->valueType = varSymbol->valueType;

	resultOp->valueType = varOp->valueType;

	pushBack(resultOp->args, varOp);

	return resultOp;
}

OpNode* handleCallOp(AstNode* opNodeAst) {
	OpNode* resultOp = createOpNode("call", OT_CALL, opNodeAst);

	AstNode* funcNameIdentifier = getItem(opNodeAst->children, 0);
	AstNode* funcName = getItem(funcNameIdentifier->children, 0);

	OpNode* funcNameOp = createOpNode(strCpy(funcName->token), OT_PLACE, funcName);
	if(funcNameOp)
		pushBack(resultOp->args, funcNameOp);

	ProgramUnit* callUnit = NULL;

	// находим unit с таким названием
	for (size_t i = 0; i < programUnitStorage->size; i++) {
		ProgramUnit* currentItem = getItem(programUnitStorage, i);
		if (strcmp(currentItem->funcSignature->name, funcNameOp->value) == 0) {
			callUnit = currentItem;
		}
	}

	if (!callUnit) {
		// обработка ошибки
		collectError(ST_ANALYZE, "func is not find", funcName->token, funcName->line);

		return NULL;
	}

	for (size_t i = 1; i < opNodeAst->children->size; i++) {
		AstNode* currentValueAst = getItem(opNodeAst->children, i);
		OpNode* currentValueOp = handleExpression(currentValueAst);

		if(currentValueOp)
			pushBack(resultOp->args, currentValueOp);
	}

	resultOp->valueType = callUnit->funcSignature->returnType;

	return resultOp;
}

OpNode* handleSliceOp(AstNode* opNodeAst) {
	OpNode* resultOp = createOpNode("index", OT_INDEX, opNodeAst);
	
	AstNode* sliceNameAst = getItem(opNodeAst->children, 1);
	AstNode* rangeListAst = getItem(opNodeAst->children, 0);
	OpNode* identifierOp = NULL;
	if (!strcmp(sliceNameAst->token, "SLICE_EXPR")) {
		identifierOp = handleSliceOp(sliceNameAst);
	}
	else {
		identifierOp = createOpNode(strCpy(sliceNameAst->token), OT_PLACE, sliceNameAst);
	}

	Array* rangeList = rangeListAst->children;
	Array dimensions = *rangeList;

	AstNode* lastDim = popBack(&dimensions);
	OpNode* lastDimOp = handleExpression(lastDim);

	OpNode* resultIndexOp = NULL;

	if (dimensions.size > 1) {
		resultIndexOp = handleMultiDimSlice(&dimensions, identifierOp);
	}
	else {
		resultIndexOp = identifierOp;
	}
	
	
	pushBack(resultOp->args, resultIndexOp);
	pushBack(resultOp->args, lastDimOp);
	
	return resultOp;
}

OpNode* handleMultiDimSlice(Array* dimensions, OpNode* identifierOp) {
	OpNode* resultOp = createOpNode("index", OT_INDEX, identifierOp->ast);

	if (dimensions->size > 0) {
		AstNode* currLastDimAst = popBack(dimensions);
		OpNode* currLastDimOp = handleExpression(currLastDimAst);
		if (dimensions->size > 0) {
			
			OpNode* resultIndexOp = handleMultiDimSlice(dimensions, identifierOp);
			pushBack(resultOp->args, resultIndexOp);
			pushBack(resultOp->args, currLastDimOp);
		}
		else {
			pushBack(resultOp->args, identifierOp);
			pushBack(resultOp->args, currLastDimOp);
			resultOp->valueType = identifierOp->valueType;
		}

	}

	return resultOp;
}

ValueType* builtInTypeIdentify(AstNode* astNode) {
	AstNode* value = getItem(astNode->children, 0);

	if (strcmp(astNode->token, "BOOL") == 0) {
		return createBuiltInType(TYPE_BOOL, astNode);
	}
	else if (strcmp(astNode->token, "STRING") == 0) {
		size_t len = strlen(value->token);
		if (len == 2) { // одни кавычки "" то массив символов
			return createBuiltInArray(TYPE_CHAR, STRING_CAPACITY, astNode);
		}
		else if (len > 2) { // константная строка "str"
			return createBuiltInPtr(TYPE_CHAR, astNode);
		}
	}
	else if (strcmp(astNode->token, "CHAR") == 0) {
		return createBuiltInType(TYPE_CHAR, astNode);
	}
	//else if (strcmp(astNode->token, "HEX") == 0) {

	//}
	//else if (strcmp(astNode->token, "BITS") == 0) {

	//}
	else if (strcmp(astNode->token, "DEC") == 0) {
		char* endPtr = NULL;
		double floatValue = strtod(value->token, &endPtr);
		if (*endPtr == '\0' && strchr(value->token, '.') != NULL) {
			return createBuiltInType(TYPE_FLOAT, astNode);
		}

		return createBuiltInType(TYPE_INT, astNode);
	}
	else {
		// Если ничего не подошло
		collectError(ST_TYPING, "unknown type", astNode->token, astNode->line);
		// обработка ошибки
		return NULL;
	}
}
