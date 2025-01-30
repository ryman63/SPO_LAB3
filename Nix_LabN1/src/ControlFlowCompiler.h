#pragma once

#include "ProgramUnit.h"
#include "CallGraph.h"
#include "VM.h"
#include "SymbolTable.h"

static VM _virtualMachine;

static SymbolTable _symbolTable;

void compile(CallGraphNode* callGraph);

int32_t generateOpTreeCode(OpNode* opNode, BasicBlock* block);
