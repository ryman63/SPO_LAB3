#include "ProgramUnit.h"

#define MAX_BUILTINS 32
#define BUILTIN_MAX_COUNT_ARGS 8

#define FUNC_ARG(nameArg, typePtr) ((FuncArg){.name = (nameArg), .type = (typePtr)})

#define FUNC_ARGS(...) __VA_ARGS__

#define REGISTER_BUILTIN(name, sourceFile, returnType, ...) \
    do { \
        FuncArg args[] = { __VA_ARGS__ }; \
        registerBuiltin(name, sizeof(args) / sizeof(FuncArg), args, returnType, sourceFile); \
    } while (0)

typedef struct BuiltInFunc {

    char* name;
    size_t countArgs;
    FuncArg args[BUILTIN_MAX_COUNT_ARGS];
    ValueType* returnType;
    char* sourceFileName;
}BuiltInFunc;

static BuiltInFunc BuiltInFuncs[MAX_BUILTINS];
static size_t BuiltInFuncCount = 0;

void registerBuiltin(
    const char* name,
    size_t argCount,
    FuncArg* funcArgs,
    ValueType* returnType,
    const char* sourceFile
);

BuiltInFunc getBuiltInFunc(size_t index);

size_t getBuiltInFuncCount();

