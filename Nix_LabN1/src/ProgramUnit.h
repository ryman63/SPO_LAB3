#pragma once

#include "Array.h"
#include "cfg.h"
#include "ValueType.h"
#include "SourceFile.h"


typedef struct FuncArg {
	char* name;
	enum ValueType type;
} FuncArg;

typedef struct FuncSignature {
	char* name;
	struct Array* FuncArgs;
} FuncSignature;

typedef struct ProgramUnit {
	struct FuncSignature* funcSignature;
	struct CfgNode* cfg;
	struct TypeRef* typeRef;

	struct SourceFile* sourceFile;
} ProgramUnit;

FuncSignature* createFuncSignature();