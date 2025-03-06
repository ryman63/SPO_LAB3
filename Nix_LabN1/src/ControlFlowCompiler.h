#pragma once

#include "ProgramUnit.h"
#include "CallGraph.h"
#include "VM.h"
#include "SymbolTable.h"
#include "stack.h"

static VM _virtualMachine;

static SymbolTable _symbolTable;

static CallStack _callStack;

static BasicBlock* blocks[1024];

static size_t counter_blocks = 0;

void compile(CallGraphNode* callGraph);

size_t calculateCountInstr();

void traverseCallGraph(CallGraphNode* root);

void generateFunctionCode(ProgramUnit* unit);

reg generateFunctionCall(OpNode* opNode, BasicBlock* block, SymbolTable* table);

reg traverseCfg(CfgNode* cfg, SymbolTable* table, reg returnReg);

reg generateBinaryOpCode(OpNode* opNode, BasicBlock* block, SymbolTable* table);

int32_t generateStackPlaceCode(OpNode* opNode, BasicBlock* block, SymbolTable* table, reg src);

reg generateReadOpCode(OpNode* opNode, BasicBlock* block, SymbolTable* table);

reg generateSetOpCode(OpNode* opNode, BasicBlock* block, SymbolTable* table);

reg generateOpTreeCode(OpNode* opNode, BasicBlock* block, SymbolTable* table);

