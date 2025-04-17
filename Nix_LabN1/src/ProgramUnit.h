#pragma once

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "Array.h"
#include "cfg.h"
#include "ValueType.h"
#include "SourceFile.h"

#define COUNT_BUILTIN_FUNCS 1
#define BUILTIN_MAX_COUNT_ARGS 8

static const char* BUILTIN_FUNC_DIRECTORY = "../stdlib";

typedef struct FuncArg {
	char* name;
	enum ValueType type;
} FuncArg;

typedef struct FuncSignature {
	char* name;
	struct Array* funcArgs;
	enum ValueType returnType;
} FuncSignature;

typedef struct ProgramUnit {
	bool isBuiltIn;

	struct AstNode* ast;
	struct FuncSignature* funcSignature;
	struct CfgNode* cfg;
	struct SymbolTable* currentTable;
	struct SourceFile* sourceFile;
} ProgramUnit;

typedef struct BuiltInFunc {

	char* name;
	size_t countArgs;
	FuncArg args[16];
	enum ValueType returnType;
	char* sourceFileName;
}BuiltInFunc;

static BuiltInFunc BuiltInFuncs[COUNT_BUILTIN_FUNCS] = {
	{"print", 1, {"str", TYPE_STRING}, TYPE_VOID, "iostream.asm"},
	//{"print", {"str", TYPE_STRING}, TYPE_VOID, NULL, NULL},
};


FuncSignature* createFuncSignature();

ProgramUnit* defineBuiltInFunc(FuncSignature* funcSignature, SourceFile* srcFile);

char* getFilePath(SourceFile* srcFile);

char* getBuiltInFuncCode(SourceFile* srcFile, const char* funcName, FILE* fileDesc);