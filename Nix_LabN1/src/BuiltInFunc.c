#include "BuiltInFunc.h"

void registerBuiltin(
	const char* name, 
	size_t argCount, 
    FuncArg* funcArgs,
	ValueType* returnType, 
	const char* sourceFile
) {
    if (BuiltInFuncCount >= MAX_BUILTINS) return;

    BuiltInFunc* func = &BuiltInFuncs[BuiltInFuncCount++];
    func->name = _strdup(name);
    func->countArgs = argCount;
    
    func->returnType = returnType;
    func->sourceFileName = _strdup(sourceFile);

    for (size_t i = 0; i < argCount; i++) {
        func->args[i].name = _strdup(funcArgs[i].name);
        func->args[i].type = funcArgs[i].type;
    }
}

BuiltInFunc getBuiltInFunc(size_t index)
{
    return BuiltInFuncs[index];
}

size_t getBuiltInFuncCount()
{
    return BuiltInFuncCount;
}


