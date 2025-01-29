#include "SymbolTable.h"

// Инициализация таблицы символов
void initSymbolTable(SymbolTable* table) {
    table->size = 0;
    table->capacity = 10; // Начальная емкость
    table->symbols = (Symbol*)malloc(table->capacity * sizeof(Symbol));
    if (!table->symbols) {
        fprintf(stderr, "Ошибка: не удалось выделить память для таблицы символов\n");
        exit(1);
    }
}

// Добавление символа в таблицу
void addSymbol(SymbolTable* table, const char* name, SymbolType type, int scope, int address, const char* dataType) {
    // Проверка на переполнение
    if (table->size >= table->capacity) {
        table->capacity *= 2;
        table->symbols = (Symbol*)realloc(table->symbols, table->capacity * sizeof(Symbol));
        if (!table->symbols) {
            fprintf(stderr, "Ошибка: не удалось перераспределить память для таблицы символов\n");
            exit(1);
        }
    }

    // Добавление нового символа
    Symbol* symbol = &table->symbols[table->size++];
    /*strncpy(symbol->name, name, MAX_NAME_LENGTH);*/
    symbol->type = type;
    symbol->scope = scope;
    symbol->address = address;
    /*strncpy(symbol->dataType, dataType, MAX_NAME_LENGTH);*/
}

// Поиск символа в таблице по имени
Symbol* findSymbol(SymbolTable* table, const char* name) {
    for (size_t i = 0; i < table->size; ++i) {
        if (strcmp(table->symbols[i].name, name) == 0) {
            return &table->symbols[i];
        }
    }
    return NULL; // Если символ не найден
}

// Освобождение памяти таблицы символов
void freeSymbolTable(SymbolTable* table) {
    free(table->symbols);
    table->symbols = NULL;
    table->size = 0;
    table->capacity = 0;
}
