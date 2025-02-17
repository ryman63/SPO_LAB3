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
	struct Array* funcArgs;
	enum ValueType returnType;
} FuncSignature;

typedef struct ProgramUnit {
	struct FuncSignature* funcSignature;
	struct CfgNode* cfg;

	struct SourceFile* sourceFile;
} ProgramUnit;

FuncSignature* createFuncSignature();