#include "FlowGraphBuilder.h"

void defineAllBuiltInFuncs(Array* programUnitStorage) {
	for (size_t i = 0; i < COUNT_BUILTIN_FUNCS; i++) {
		BuiltInFunc srcFunc = BuiltInFuncs[i];
		ProgramUnit builtInFunc;
		builtInFunc.funcSignature = createFuncSignature();
		Array* funcArgsArray = buildArray(sizeof(FuncArg), BUILTIN_MAX_COUNT_ARGS);
		for (size_t j = 0; j < srcFunc.countArgs; j++) {
			FuncArg* arg = malloc(sizeof(FuncArg));
			arg->name = srcFunc.args[j].name;
			arg->type = srcFunc.args[j].type;
			pushBack(funcArgsArray, &arg);
		}
		builtInFunc.funcSignature->funcArgs = funcArgsArray;
		builtInFunc.funcSignature->name = srcFunc.name;
		builtInFunc.funcSignature->returnType = srcFunc.returnType;
		builtInFunc.cfg = NULL;
		builtInFunc.isBuiltIn = true;
		builtInFunc.currentTable = NULL;
		builtInFunc.sourceFile = GetSrcFile(srcFunc.sourceFileName, BUILTIN_FUNC_DIRECTORY);
		pushBack(programUnitStorage, &builtInFunc);
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

		// BuiltIn �������
		defineAllBuiltInFuncs(programUnitStorage);

		// ���������������� �������
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

ValueType getType(AstNode* rootTypeRefAst) {
	AstNode* typeOfType = getItem(rootTypeRefAst->children, 0);
	if (!strcmp(typeOfType->token, "BUILTIN")) {
		AstNode* builtInType = getItem(typeOfType->children, 0);
		if (!strcmp(builtInType->token, "int")) {
			return TYPE_INT;
		}
		else if (!strcmp(builtInType->token, "float")) {
			return TYPE_FLOAT;
		}
		else if (!strcmp(builtInType->token, "bool")) {
			return TYPE_BOOL;
		}
		else if (!strcmp(builtInType->token, "string")) {
			return TYPE_STRING;
		}
	}
	else if (!strcmp(typeOfType->token, "ARRAY")) {
		return TYPE_ARRAY;
	}
	else if (!strcmp(typeOfType->token, "CUSTOM")) {
		return TYPE_CUSTOM;
	}
	return TYPE_ERROR;
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
				
				ValueType type = getType(argType);
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
		signature->returnType = TYPE_VOID;
	

	return signature;
}

CfgNode* handleFunctionBody(AstNode* functionBodyAst, Array* funcArgs) {
	breakTargets = buildArray(sizeof(CfgNode), 4);

	if (!functionBodyAst)
		return NULL;

	// ��������� ������� ����������� �� ���������� �������
	for (size_t i = 0; i < funcArgs->size; i++) {
		FuncArg* arg = getItem(funcArgs, i);
		addSymbol(currentTable, arg->name, SYMBOL_FUNC_PARAM, -1, arg->type);
	}

	CfgNode* entryNode = createCfgNode("function entry", functionBodyAst);
	CfgNode* currentNode = entryNode;
	CfgNode* lastCfgNode = entryNode;

	for (size_t i = 1; i < functionBodyAst->children->size; i++) {
		AstNode* statement = getItem(functionBodyAst->children, i);
		CfgNode* statementNode = handleStatement(statement, &lastCfgNode);

		if (statementNode) {
			if (!strcmp(statementNode->label, "loop statement"))
				lastCfgNode->uncondJump = statementNode;
			currentNode->condJump = statementNode;
			currentNode = lastCfgNode;
		}
	}

	//lastCfgNode = currentNode;

	CfgNode* exitNode = createCfgNode("function exit", NULL);
	lastCfgNode->condJump = exitNode;

	freeArray(&breakTargets);

	return entryNode;
}


CfgNode* handleStatement(AstNode* rootStatementAst, CfgNode** lastCfgNode) {
	if (!rootStatementAst) 
		return NULL;

	AstNode* typeOfStatement = getItem(rootStatementAst->children, 0);

	if(typeOfStatement == NULL) {
		// error

		return NULL;
	}

	if (!strcmp(typeOfStatement->token, "BLOCK_STATEMENT")) {
		return handleBlockStatement(typeOfStatement, lastCfgNode);
	}
	else if (!strcmp(typeOfStatement->token, "IF_STATEMENT")) {
		return handleConditionStatement(typeOfStatement, lastCfgNode);
	}
	else if (!strcmp(typeOfStatement->token, "LOOP_STATEMENT")) {
		return handleLoopStatement(typeOfStatement, lastCfgNode);
	}
	else if (!strcmp(typeOfStatement->token, "BREAK_STATEMENT")) {
		handleBreakStatement(typeOfStatement, lastCfgNode);
	}
	else if (!strcmp(typeOfStatement->token, "REPEAT_STATEMENT")) {
		handleRepeatStatement(typeOfStatement, lastCfgNode);
	}
	else if (!strcmp(typeOfStatement->token, "EXPRESSION_STATEMENT")) {
		handleExpressionStatement(typeOfStatement, lastCfgNode);
	}
	else {
		// ��������� ������
		collectError(ST_ANALYZE, "Unhandled statement", typeOfStatement->token, typeOfStatement->line);

		return NULL;
	}
}

CfgNode* handleBlockStatement(AstNode* statementNodeAst, CfgNode** lastCfgNode) {
	CfgNode* newBlock = createCfgNode("block statement", statementNodeAst);
	CfgNode* currentBlock = newBlock;
	if (statementNodeAst) {
		for (size_t i = 0; i < statementNodeAst->children->size; i++) {
			AstNode* childStatement = getItem(statementNodeAst->children, i);
			CfgNode* result = handleStatement(childStatement, lastCfgNode);
			if (result && !strcmp(result->label, "loop statement"))
			{
				CfgNode* lstNode = *lastCfgNode;
				lstNode->uncondJump = result;
			}
			currentBlock->condJump = result;
			currentBlock = *lastCfgNode;
		}
	}
	else {
		// ��������� ������
		collectError(ST_ANALYZE, "statement is", "NULL", -1);

		return NULL;
	}

	*lastCfgNode = currentBlock;

	return newBlock;
}

CfgNode* handleConditionStatement(AstNode* statementNodeAst, CfgNode** lastCfgNode) {
	CfgNode* emptyBlock = createCfgNode("empty", NULL);
	CfgNode* newBlock = createCfgNode("condition statement", statementNodeAst);
	CfgNode* condLastBlock = NULL;
	CfgNode* unCondLastBlock = NULL;
	Array* children = statementNodeAst->children;

	AstNode* exprNode = getItem(children, 0);
	newBlock->opTree = handleExpression(getItem(exprNode->children, 0));

	if (children->size == 2) {
		AstNode* condStatement = getItem(children, 1);
		newBlock->condJump = handleStatement(condStatement, lastCfgNode);
		condLastBlock = *lastCfgNode;
	}
	else if (children->size == 3) {
		AstNode* condStatement = getItem(children, 1);
		AstNode* uncondStatement = getItem(children, 2);
		newBlock->condJump = handleStatement(condStatement, lastCfgNode);
		condLastBlock = *lastCfgNode;
		newBlock->uncondJump = handleStatement(uncondStatement, lastCfgNode);
		unCondLastBlock = *lastCfgNode;
	}
	else {
		// ��������� ������
		char childrens[4];

		_itoa_s(children->size, childrens, 4, 10);

		collectError(ST_ANALYZE, "condition ast children incorrect", childrens, statementNodeAst->line);

		return NULL;
	}

	if(children->size == 2)
		newBlock->uncondJump = emptyBlock;
	else {
		unCondLastBlock->condJump = emptyBlock;
	}

	condLastBlock->condJump = emptyBlock;


	*lastCfgNode = emptyBlock;

	return newBlock;
}

CfgNode* handleLoopStatement(AstNode* statementNodeAst, CfgNode** lastCfgNode) {
	CfgNode* newBlock = createCfgNode("loop statement", statementNodeAst);
	CfgNode* exitBlock = createCfgNode("exit loop", NULL);
	CfgNode* currentBlock = newBlock;
	Array* children = statementNodeAst->children;

	AstNode* exprNode = getItem(children, 0);
	newBlock->opTree = handleExpression(getItem(exprNode->children, 0));
	pushBack(breakTargets, exitBlock);
	if (children->size > 1) {
		for (size_t i = 1; i < children->size; i++) {
			AstNode* statement = getItem(children, i);
			CfgNode* result = handleStatement(statement, lastCfgNode);
			currentBlock->condJump = result;
			currentBlock = *lastCfgNode;
			if (result->label == "break statement") {
				break;
			}
		}
	}
	else {
		// ��������� ������
		char childrens[4];

		_itoa_s(children->size, childrens, 4, 10);

		collectError(ST_ANALYZE, "loop ast children incorrect", childrens, statementNodeAst->line);

		return NULL;
	}

	currentBlock->condJump = exitBlock;

	//newBlock->uncondJump = exitBlock;

	exitBlock->condJump = newBlock;

	*lastCfgNode = exitBlock;

	return newBlock;
}

CfgNode* handleRepeatStatement(AstNode* typeOfStatement, CfgNode** lastCfgNode) {
	CfgNode* enterBlock = createCfgNode("enter loop", NULL);
	CfgNode* newBlock = createCfgNode("repeat statement", typeOfStatement);
	CfgNode* currentBlock = enterBlock;
	Array* children = typeOfStatement->children;

	AstNode* exprNode = getItem(children, 1);
	newBlock->opTree = handleExpression(getItem(exprNode->children, 0));
	pushBack(breakTargets, newBlock);
	if (children->size > 1) {
		for (size_t i = 0; i < children->size - 1; i++) {
			AstNode* statement = getItem(children, i);
			CfgNode* result = handleStatement(statement, lastCfgNode);
			currentBlock->condJump = result;
			currentBlock = *lastCfgNode;
			if (result->label == "break statement") {
				break;
			}
		}
	}
	else {
		// ��������� ������
		char childrens[4];

		_itoa_s(children->size, childrens, 4, 10);

		collectError(ST_ANALYZE, "repeat ast children incorrect", childrens, typeOfStatement->line);

		return NULL;
	}
	
	currentBlock->condJump = newBlock;

	newBlock->uncondJump = enterBlock;

	*lastCfgNode = newBlock;

	return enterBlock;
}

CfgNode* handleBreakStatement(AstNode* statementNodeAst, CfgNode** lastCfgNode) {
	CfgNode* newBlock = createCfgNode("break statement", statementNodeAst);

	if (breakTargets->size == 0) {
		// ��������� ������
		collectError(ST_ANALYZE, "not found target", "break statement", statementNodeAst->line);

		return NULL;
	}

	CfgNode* result = popBack(breakTargets);
	newBlock->condJump = result;
	*lastCfgNode = result;

	return newBlock;
}

CfgNode* handleExpressionStatement(AstNode* statementNodeAst, CfgNode** lastCfgNode) {
	CfgNode* newBlock = createCfgNode("expression statement", statementNodeAst);

	AstNode* expressionStatementAst = getItem(statementNodeAst->children, 0);

	newBlock->opTree = handleExpression(getItem(expressionStatementAst->children, 0));
	
	*lastCfgNode = newBlock;

	return newBlock;
}


OpNode* handleExpression(AstNode* exprNode) {

	char* firstToken = exprNode->token;

	if (!strcmp(firstToken, "=")) {
		return handleSet(exprNode);
	}
	else if (!strcmp(firstToken, "+")) {
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
	else if(!strcmp(firstToken, "LITERAL_EXPR")) {
		return handleLiteralOp(getItem(exprNode->children, 0));
	}
	else if (!strcmp(firstToken, "CALL_EXPR")) {
		return handleCallOp(exprNode);
	}
	else if (!strcmp(firstToken, "SLICE_EXPR")) {
		return handleSliceOp(exprNode);
	}
	else {
		return handleVarOp(exprNode);
	}
	
	return NULL;
}

OpNode* handleSet(AstNode* opNodeAst) {
	AstNode* lValue = getItem(opNodeAst->children, 0);
	AstNode* rValue = getItem(opNodeAst->children, 1);

	OpNode* resultOp = createOpNode("set", OT_WRITE, opNodeAst);

	OpNode* lValueOp = NULL;
	if (!strcmp(lValue->token, "SLICE_EXPR"))
		lValueOp = handleSliceOp(lValue);
	else
		lValueOp = createOpNode(strCpy(lValue->token), OT_PLACE, opNodeAst);

	OpNode* rValueOp = handleExpression(rValue);

	lValueOp->valueType = rValueOp->valueType;

	pushBack(resultOp->args, lValueOp);

	
	

	if(rValueOp)
		pushBack(resultOp->args, rValueOp);
	else {
		// ��������� ������
		collectError(ST_ANALYZE, "is not found", rValue->token, rValue->line);

		return NULL;
	}

	if(!findSymbol(currentTable, lValueOp->value))
		addSymbol(currentTable, lValueOp->value, SYMBOL_VARIABLE, -1, lValueOp->valueType);

	return resultOp;
}

OpNode* handleBinaryOp(AstNode* opNodeAst) {
	OpNode* resultOp = createOpNode(strCpy(opNodeAst->token), OT_BINARY, opNodeAst);

	AstNode* lValue = getItem(opNodeAst->children, 0);
	AstNode* rValue = getItem(opNodeAst->children, 1);

	OpNode* lValueOp = handleExpression(lValue);
	OpNode* rValueOp = handleExpression(rValue);
	
	resultOp->valueType = compareTypes(lValueOp->valueType, rValueOp->valueType);

	if(lValueOp)
		pushBack(resultOp->args, lValueOp);
	if(rValueOp)
		pushBack(resultOp->args, rValueOp);

	return resultOp;
}

OpNode* handleLiteralOp(AstNode* varOrLit) {
	OpNode* resultOp = createOpNode("const", OT_CONST, varOrLit);

	OpNode* litOp = createOpNode(strCpy(varOrLit->token), OT_LITERAL, varOrLit);

	pushBack(resultOp->args, litOp);

	litOp->valueType = typeIdentify(varOrLit->token);
	resultOp->valueType = litOp->valueType;

	return resultOp;
}

OpNode* handleVarOp(AstNode* var) {
	Symbol* varSymbol = findSymbol(currentTable, var->token);

	if (!varSymbol) {
		// ��������� ������ - ���������� �� ����������������
		collectError(ST_ANALYZE, "unknown variable", var->token, var->line);

		return NULL;
	}

	OpNode* resultOp = createOpNode("read", OT_READ, var);

	OpNode* varOp = createOpNode(strCpy(var->token), OT_PLACE, var);

	varOp->valueType = varSymbol->valueType;

	resultOp->valueType = varOp->valueType;

	pushBack(resultOp->args, varOp);

	return resultOp;
}

OpNode* handleCallOp(AstNode* opNodeAst) {
	OpNode* resultOp = createOpNode("call", OT_CALL, opNodeAst);

	AstNode* funcNameAst = getItem(opNodeAst->children, 0);

	OpNode* funcNameOp = createOpNode(strCpy(funcNameAst->token), OT_PLACE, funcNameAst);
	if(funcNameOp)
		pushBack(resultOp->args, funcNameOp);

	ProgramUnit* callUnit = NULL;

	// ������� unit � ����� ���������
	for (size_t i = 0; i < programUnitStorage->size; i++) {
		ProgramUnit* currentItem = getItem(programUnitStorage, i);
		if (strcmp(currentItem->funcSignature->name, funcNameOp->value) == 0) {
			callUnit = currentItem;
		}
	}

	if (!callUnit) {
		// ��������� ������
		collectError(ST_ANALYZE, "func is not find", funcNameAst->token, funcNameAst->line);

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