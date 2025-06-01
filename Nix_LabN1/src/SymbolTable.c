#include "SymbolTable.h"


// ������������� ������� ��������
void initSymbolTable(SymbolTable* table) {
    table->size = 0;
    table->capacity = 10; // ��������� �������
    table->symbols = (Symbol*)malloc(table->capacity * sizeof(Symbol));
    if (!table->symbols) {
        fprintf(stderr, "������: �� ������� �������� ������ ��� ������� ��������\n");
        exit(1);
    }
}

// ���������� ������� � �������
void addSymbol(SymbolTable* table, const char* name, SymbolType type, int address, ValueType* valueType) {
    // �������� �� ������������
    if (table->size >= table->capacity) {
        table->capacity *= 2;
        table->symbols = (Symbol*)realloc(table->symbols, table->capacity * sizeof(Symbol));
        if (!table->symbols) {
            fprintf(stderr, "������: �� ������� ���������������� ������ ��� ������� ��������\n");
            exit(1);
        }
    }

    // ���������� ������ �������
    Symbol* symbol = &table->symbols[table->size++];
    strncpy_s(symbol->name, MAX_NAME_LENGTH, name, MAX_NAME_LENGTH);
    symbol->type = type;
    symbol->address = address;
    symbol->valueType = valueType;
}

// ����� ������� � ������� �� �����
Symbol* findSymbol(SymbolTable* table, const char* name) {
    for (size_t i = 0; i < table->size; ++i) {
        if (strcmp(table->symbols[i].name, name) == 0) {
            return &table->symbols[i];
        }
    }
    return NULL; // ���� ������ �� ������
}

// ������������ ������ ������� ��������
void freeSymbolTable(SymbolTable* table) {
    free(table->symbols);
    table->symbols = NULL;
    table->size = 0;
    table->capacity = 0;
}
