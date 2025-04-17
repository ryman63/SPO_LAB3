#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_COLLECTOR_SIZE 1024

typedef enum Stage {
	ST_ANALYZE,
	ST_TYPING,
	ST_COMPILE,
	ST_GENERATE,
} Stage;

typedef struct Error {
	int line;
	char* description;
	char* data;
	enum Stage stage;
} Error;

typedef struct ErrorCollector {
	size_t size;
	Error* errors[ERROR_COLLECTOR_SIZE];
} ErrorCollector;

static ErrorCollector collector;

Error* createError(enum Stage stage, char* description, char* value, int line);

void collectError(enum Stage stage, char* description, char* value, int line);

void printErrors();

void printError(Error* error);

char* getStageStr(enum Stage stage);