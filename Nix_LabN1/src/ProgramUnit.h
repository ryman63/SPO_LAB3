#pragma once

#include "Array.h"
#include "cfg.h"
#include "ValueType.h"
#include "SourceFile.h"

typedef enum Type {
	E_BUILTIN,
	E_CUSTOM,
	E_ARRAY
};

typedef struct CustomType {
	char* nameOfType;
	size_t sizeOfType;
};

typedef struct BuiltInType {
	enum ValueType type;
	size_t sizeOfType;
} BuiltInType;

typedef struct ArrayType {
	size_t dim;
	struct BuiltInType basicType;
};

typedef struct TypeRef {
	void* ref;
	enum Type type;
} TypeRef;

typedef struct FuncArg {
	char* name;
	struct TypeRef* typeRef;
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