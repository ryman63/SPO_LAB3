#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE_OF_INT 4
#define SIZE_OF_CHAR 1
#define SIZE_OF_BOOL 1
#define SIZE_OF_FLOAT 4
#define SIZE_OF_PTR 4

typedef enum ValueType{
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_PTR,
	TYPE_CHAR,
	TYPE_BOOL,
	TYPE_CUSTOM,
	TYPE_VOID,
	TYPE_ERROR
} ValueType;

ValueType compareTypes(ValueType type1, ValueType type2);

ValueType typeIdentify(char* value);

int getTypeSize(ValueType type);