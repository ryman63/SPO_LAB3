#include "ProgramUnit.h"

FuncSignature* createFuncSignature() {
    FuncSignature* signature = malloc(sizeof(FuncSignature));
    signature->funcArgs = buildArray(sizeof(FuncArg), 2);
    signature->name = NULL;
}

ProgramUnit* defineBuiltInFunc(FuncSignature* funcSignature, SourceFile* srcFile) {
    ProgramUnit* unit = malloc(sizeof(ProgramUnit));
    unit->funcSignature = funcSignature;
    unit->sourceFile = srcFile;
    unit->isBuiltIn = true;
    return unit;
}

char* getFilePath(SourceFile* srcFile)
{
    size_t filePathLen = strlen(srcFile->directoryPath) + strlen(srcFile->fileName) + 2;
    char* filePath = malloc(filePathLen);
    snprintf(filePath, filePathLen, "%s/%s", srcFile->directoryPath, srcFile->fileName);
    return filePath;
}

char* getBuiltInFuncCode(SourceFile* srcFile, const char* funcName, FILE* fileDesc) {
    const size_t bufferResultSize = 4096;
    char* result = malloc(bufferResultSize);

    const size_t MAX_LINE_SIZE = 512;
    char* filePath = getFilePath(srcFile);
    char* funcNameMark = malloc(MAX_LINE_SIZE);
    snprintf(funcNameMark, MAX_LINE_SIZE, "%s:", funcName);
    FILE* builtInFile = fopen(filePath, "r");

    char* currLine = malloc(MAX_LINE_SIZE);

    result[0] = '\0'; // Пустая строка
    bool isFound = false;

    while (fgets(currLine, MAX_LINE_SIZE, builtInFile)) {

        char* trimmed = currLine;
        while (*trimmed == ' ' || *trimmed == '\t') trimmed++; // убираем отступы
        if (!isFound) {
            if (strncmp(trimmed, funcNameMark, strlen(funcNameMark)) == 0) {
                isFound = true;
                strncat_s(result, bufferResultSize, currLine, _TRUNCATE);
            }
        } 
        else {
            strncat_s(result, bufferResultSize, currLine, _TRUNCATE);
            if (strstr(trimmed, "ret")) {
                break;
            }
        }
    }
    
    free(filePath);
    free(funcNameMark);
    fclose(builtInFile);

    if (!isFound) {
        free(result);
        return NULL;
    }

    return result;
}
