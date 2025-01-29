#include "ProgramUnit.h"

FuncSignature* createFuncSignature() {
    FuncSignature* signature = malloc(sizeof(FuncSignature));
    signature->FuncArgs = buildArray(sizeof(FuncArg), 2);
    signature->name = NULL;
}
