#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

const char LINE[7] = " Line:";
const char ERROR[8] = " Error:";

typedef struct Error {
	int line;
	char* data;
	size_t dataSize;
};

typedef struct ErrorCollector {
	size_t size;
	Error* errors;
};

void printErrors(ErrorCollector* collector) {
	const int RESULT_STRING_SIZE = 512;

	for (size_t i = 0; i < collector->size; i++) {
		Error currError = collector->errors[i];
		char lineStr[16];
		_itoa_s(currError.line, lineStr, 16, 10);

		char resultStr[RESULT_STRING_SIZE];
		strcpy_s(resultStr, RESULT_STRING_SIZE, currError.data);
		strcpy_s(resultStr, RESULT_STRING_SIZE, LINE);
		strcpy_s(resultStr, RESULT_STRING_SIZE, lineStr);
		strcpy_s(resultStr, RESULT_STRING_SIZE, ERROR);
		strcpy_s(resultStr, RESULT_STRING_SIZE, currError.data);
		printf(resultStr);
	}
}