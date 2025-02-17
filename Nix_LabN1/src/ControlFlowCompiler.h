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

void generateFunctionCall(Array* args, BasicBlock* block);

BasicBlock* traverseCfg(CfgNode* cfg);

void generateBinaryOpCode(OpNode* opNode, BasicBlock* block);

char* getLiteral(OpNode* opNode);

int32_t generatePlaceOpCode(OpNode* opNode, BasicBlock* block);

int32_t generateReadOpCode(OpNode* opNode, BasicBlock* block);

int32_t generateSetOpCode(OpNode* opNode, BasicBlock* block);

int32_t generateOpTreeCode(OpNode* opNode, BasicBlock* block);

