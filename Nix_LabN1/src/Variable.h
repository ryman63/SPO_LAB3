#pragma once
#include "ValueType.h"
#include "ProgramUnit.h"

#define VARIABLE_SIZE 4

typedef struct Variable {
	char* name;
	ValueType* type;
	int address;
	bool isInit;
} Variable;