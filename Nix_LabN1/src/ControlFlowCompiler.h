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

int32_t generateOpTreeCode(OpNode* opNode, BasicBlock* block);
