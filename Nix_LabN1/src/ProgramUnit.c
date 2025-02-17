#include "ProgramUnit.h"

FuncSignature* createFuncSignature() {
    FuncSignature* signature = malloc(sizeof(FuncSignature));
    signature->funcArgs = buildArray(sizeof(FuncArg), 2);
    signature->name = NULL;
}
