#pragma once

#include "ProgramUnit.h"
#include "myParser.h"
#include "CallGraph.h"
#include "SymbolTable.h"
#include "ErrorCollector.h"

Array* breakTargets;
SymbolTable* currentTable;
Array* programUnitStorage;

CallGraphNode* analysis(Array* srcFiles, char* outputDir, Array* astList);

ValueType getType(AstNode* rootAstNode);

FuncSignature* buildFuncSignature(AstNode* rootFuncAst);

//CfgNode* buildCfg(AstNode* rootFuncAst);

CfgNode* handleStatement(AstNode* rootStatementAst, CfgNode* entryNode, CfgNode* exitNode);

CfgNode* handleBlockStatement(AstNode* statementNodeAst, CfgNode* entryNode, CfgNode* exitNode);

CfgNode* handleConditionStatement(AstNode* statementNodeAst, CfgNode* entryNode, CfgNode* exitNode);

CfgNode* handleLoopStatement(AstNode* statementNodeAst, CfgNode* entryNode, CfgNode* exitNode);

CfgNode* handleRepeatStatement(AstNode* typeOfStatement, CfgNode* entryNode, CfgNode* exitNode);

CfgNode* handleExpressionStatement(AstNode* statementNodeAst, CfgNode* entryNode, CfgNode* exitNode);

CfgNode* handleBreakStatement(AstNode* statementNodeAst, CfgNode* entryNode, CfgNode* exitNode);

OpNode* handleExpression(AstNode* exprNode);

//CfgNode* handleFunction(AstNode* functionAst);

CfgNode* handleFunctionBody(AstNode* functionBodyAst, Array* funcArgs);

OpNode* handleSet(AstNode* opNodeAst);

OpNode* handleBinaryOp(AstNode* opNodeAst);

OpNode* handleLiteralOp(AstNode* varOrLit);
OpNode* handleVarOp(AstNode* varOrLit);

OpNode* handleCallOp(AstNode* opNodeAst);

OpNode* handleSliceOp(AstNode* opNodeAst);

OpNode* handleMultiDimSlice(Array* dimensions, OpNode* identifierOp);