#pragma once

#include "ProgramUnit.h"
#include "myParser.h"
#include "CallGraph.h"

Array* breakTargets;

CallGraphNode* analysis(Array* srcFiles, char* outputDir);

TypeRef* buildTypeRef(AstNode* rootTypeRefAst);

FuncSignature* buildFuncSignature(AstNode* rootFuncAst);

//CfgNode* buildCfg(AstNode* rootFuncAst);

CfgNode* handleStatement(AstNode* rootStatementAst, CfgNode** lastCfgNode);

CfgNode* handleBlockStatement(AstNode* statementNodeAst, CfgNode** lastCfgNode);

CfgNode* handleConditionStatement(AstNode* statementNodeAst, CfgNode** lastCfgNode);

CfgNode* handleLoopStatement(AstNode* statementNodeAst, CfgNode** lastCfgNode);

CfgNode* handleRepeatStatement(AstNode* typeOfStatement, CfgNode** lastCfgNode);

CfgNode* handleExpressionStatement(AstNode* statementNodeAst, CfgNode** lastCfgNode);

CfgNode* handleBreakStatement(AstNode* statementNodeAst, CfgNode** lastCfgNode);

OpNode* handleExpression(AstNode* exprNode);

//CfgNode* handleFunction(AstNode* functionAst);

CfgNode* handleFunctionBody(AstNode* functionBodyAst);

OpNode* handleSet(AstNode* opNodeAst);

OpNode* handleBinaryOp(AstNode* opNodeAst);

OpNode* handleLiteralOp(AstNode* varOrLit);
OpNode* handleVarOp(AstNode* varOrLit);

OpNode* handleCallOp(AstNode* opNodeAst);

OpNode* handleSliceOp(AstNode* opNodeAst);

OpNode* handleMultiDimSlice(Array* dimensions, OpNode* identifierOp);