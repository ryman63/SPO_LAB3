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
void addSymbol(SymbolTable* table, const char* name, SymbolType type, int scope, int address, const char* dataType) {
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
    /*strncpy(symbol->name, name, MAX_NAME_LENGTH);*/
    symbol->type = type;
    symbol->scope = scope;
    symbol->address = address;
    /*strncpy(symbol->dataType, dataType, MAX_NAME_LENGTH);*/
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
