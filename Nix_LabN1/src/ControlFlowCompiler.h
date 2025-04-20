#pragma once

#include "ProgramUnit.h"
#include "CallGraph.h"
#include "SymbolTable.h"
#include "stack.h"
#include "Module.h"
#include "ErrorCollector.h"

#define COUNT_MODULES 4
#define CFG_MAX_NODES 10000

Array* compile(CallGraphNode* callGraph);

//size_t calculateCountInstr();

void traverseCallGraph(CallGraphNode* root, Array* modules, SymbolTable* globalTable);

Module* generateFunctionCode(ProgramUnit* unit, MachineState* state, SymbolTable* globalTable);

reg traverseCfg(CfgNode* cfg, bool visited[], Module* genModule, MachineState* state, reg returnReg);

reg generateFunctionCall(OpNode* opNode, ExprContext* ctx);

reg generateBinaryOpCode(OpNode* opNode, ExprContext* ctx);

//int32_t generateStackPlaceCode(OpNode* opNode, ExprContext* ctx);

reg generateReadOpCode(OpNode* opNode, ExprContext* ctx);

reg generateSetOpCode(OpNode* opNode, ExprContext* ctx);

reg generateOpTreeCode(OpNode* opNode, ExprContext* ctx);

