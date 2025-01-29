#pragma once

#include "ProgramUnit.h"
#include "CallGraph.h"
#include "VM.h"
#include "SymbolTable.h"

static VM _virtualMachine;

static SymbolTable _symbolTable;

void compile(CallGraphNode* callGraph);

void generateOpTreeCode(OpNode* opNode, BasicBlock* block);
