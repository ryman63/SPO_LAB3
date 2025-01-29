#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ������������ ����� ����� �������
#define MAX_NAME_LENGTH 64

// ���� ��������
typedef enum SymbolType {
    SYMBOL_VARIABLE,   // ����������
    SYMBOL_FUNCTION,   // �������
    SYMBOL_CONSTANT    // ���������
} SymbolType;

// ���������� � �������
typedef struct Symbol {
    char name[MAX_NAME_LENGTH]; // ��� �������
    enum SymbolType type;            // ��� �������
    int scope;                  // ������� ������� ���������
    int address;                // ����� ��� �������� � ������
    char dataType[MAX_NAME_LENGTH]; // ��� ������ (��������, int, float)
} Symbol;

// ������� ��������
typedef struct SymbolTable {
    struct Symbol* symbols;  // ������ ��������
    size_t size;      // ������� ���������� ��������
    size_t capacity;  // ������������ ������� �������
} SymbolTable;

void initSymbolTable(SymbolTable* table);
void addSymbol(SymbolTable* table, const char* name, SymbolType type, int scope, int address, const char* dataType);
Symbol* findSymbol(SymbolTable* table, const char* name);
void freeSymbolTable(SymbolTable* table);