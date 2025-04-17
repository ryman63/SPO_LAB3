#include "ErrorCollector.h"

char* getStageStr(enum Stage stage) {
    switch (stage)
    {
    case ST_ANALYZE: return "Analyze";
        break;
    case ST_COMPILE: return "Compile";
        break;
    case ST_GENERATE: return "GenerateCode";
        break;
    case ST_TYPING: return "Typing";
        break;
    default: return NULL;
    }
}

Error* createError(enum Stage stage, char* description, char* value, int line)
{
    Error* error = malloc(sizeof(Error));
    error->stage = stage;
    error->description = _strdup(description);
    error->data = _strdup(value);
    error->line = line;
}

void collectError(enum Stage stage, char* description, char* value, int line)
{
    Error* error = createError(stage, description, value, line);
    collector.errors[collector.size] = error;
    collector.size++;
}

void printErrors() {
    for (size_t i = 0; i < collector.size; i++) {
        Error* currError = collector.errors[i];
        printError(currError);
    }
}

void printError(Error* error) {
    const int RESULT_STRING_SIZE = 512;

    char line[8];
    _itoa_s(error->line, line, 8, 10);

    char* resultStr = malloc(sizeof(RESULT_STRING_SIZE));
    snprintf(resultStr, RESULT_STRING_SIZE, "[%s] Error: %s, %s on line: %s", getStageStr(error->stage), error->description, error->data, line);
    printf("%s\n", resultStr);
}