#pragma once

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "Array.h"
#include "cfg.h"
#include "ValueType.h"
#include "SourceFile.h"

static const char* BUILTIN_FUNC_DIRECTORY = "../stdlib";

typedef struct FuncArg {
	char* name;
	ValueType* type;
} FuncArg;

typedef struct FuncSignature {
	char* name;
	struct Array* funcArgs;
	ValueType* returnType;
} FuncSignature;

typedef struct ProgramUnit {
	bool isBuiltIn;

	struct AstNode* ast;
	struct FuncSignature* funcSignature;
	struct CfgNode* cfg;
	struct SymbolTable* currentTable;
	struct SourceFile* sourceFile;
} ProgramUnit;



FuncSignature* createFuncSignature();

ProgramUnit* defineBuiltInFunc(FuncSignature* funcSignature, SourceFile* srcFile);

char* getFilePath(SourceFile* srcFile);

char* getBuiltInFuncCode(SourceFile* srcFile, const char* funcName, FILE* fileDesc);

ProgramUnit createBuiltInProgramUnit(Array* funcArgs, char* funcName, ValueType* valueType, char* sourceFileName);