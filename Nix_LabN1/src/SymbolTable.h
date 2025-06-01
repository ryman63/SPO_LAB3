#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ValueType.h"

// ������������ ����� ����� �������
#define MAX_NAME_LENGTH 64

// ���� ��������
typedef enum SymbolType {
    SYMBOL_VARIABLE,   // ����������
    SYMBOL_FUNC_PARAM,   // ����������
    SYMBOL_FUNCTION,   // �������
    SYMBOL_CONSTANT    // ���������
} SymbolType;

typedef enum SymbolLocation {
    LOC_REG,
    LOC_STACK,
    LOC_HEAP
};

// ���������� � �������
typedef struct Symbol {
    char name[MAX_NAME_LENGTH]; // ��� �������
    enum SymbolType type;            // ��� �������
//    int scope;                  // ������� ������� ���������
    int address;                // ����� ��� �������� � ������
    ValueType* valueType; // ��� ������ (��������, int, float)
    enum SymbolLocation location;
} Symbol;

// ������� ��������
typedef struct SymbolTable {
    struct Symbol* symbols;  // ������ ��������
    size_t size;      // ������� ���������� ��������
    size_t capacity;  // ������������ ������� �������
} SymbolTable;

void initSymbolTable(SymbolTable* table);
void addSymbol(SymbolTable* table, const char* name, SymbolType type, int address, ValueType* valueType);
Symbol* findSymbol(SymbolTable* table, const char* name);
void freeSymbolTable(SymbolTable* table);