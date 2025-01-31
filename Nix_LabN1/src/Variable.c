#include "Variable.h"

Variable* initVariable(char* name, ValueType type) {
	Variable* var = malloc(sizeof(Variable));
	var->name = name;
	var->type = type;
}