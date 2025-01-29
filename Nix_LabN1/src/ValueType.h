#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum ValueType{
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_STRING,
	TYPE_ARRAY,
	TYPE_BOOL,
	TYPE_ERROR
} ValueType;

ValueType compareTypes(ValueType type1, ValueType type2);

ValueType typeIdentify(char* value);