#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "ErrorCollector.h"
#include "ast.h"

#define SIZE_OF_INT 4
#define SIZE_OF_CHAR 1
#define SIZE_OF_BOOL 1
#define SIZE_OF_FLOAT 4
#define SIZE_OF_PTR 4
#define SIZE_OF_VOID 0

#define STRING_CAPACITY 256

typedef enum BuiltInType {
    TYPE_BYTE,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_CHAR,
	TYPE_BOOL,
	TYPE_VOID,
	TYPE_ERROR
} BuiltInType;

typedef enum TypeKind {
    TYPE_KIND_BUILTIN,
    TYPE_KIND_CUSTOM,
    TYPE_KIND_POINTER,
    TYPE_KIND_ARRAY,
    // возможно, TYPE_FUNCTION, TYPE_STRUCT и др.
} TypeKind;

typedef struct ValueType {
    TypeKind kind;
    union {
        struct {
            BuiltInType type;
            size_t size;
        } builtin;
        struct {
            char* name;
            size_t size;
        } custom;
        struct {
            struct ValueType* to;
        } pointer;
        struct {
            struct ValueType* of;
            size_t size;
        } array;
    };
} ValueType;

ValueType* compareTypes(ValueType* left, ValueType* right, AstNode* astNode);

ValueType* tryCoerce(ValueType* left, ValueType* right);

ValueType* typeIdentify(char* value, AstNode* astNode);

int getBuiltInTypeSize(BuiltInType type);

ValueType* createBuiltInPtr(enum BuiltInType srcType, AstNode* astNode);

ValueType* createBuiltInType(enum BuiltInType type, AstNode* astNode);

ValueType* createBuiltInArray(enum BuiltInType srcType, size_t size, AstNode* astNode);

BuiltInType getBuiltInTypeFromString(char* typeStr);

TypeKind getTypeKindFromString(char* typeKindStr);

bool isConstString(ValueType* vType);

size_t getSizeOfArray(ValueType* vType);

size_t getSizeOfType(ValueType* vType);