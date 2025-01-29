#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Максимальная длина имени символа
#define MAX_NAME_LENGTH 64

// Типы символов
typedef enum SymbolType {
    SYMBOL_VARIABLE,   // Переменная
    SYMBOL_FUNCTION,   // Функция
    SYMBOL_CONSTANT    // Константа
} SymbolType;

// Информация о символе
typedef struct Symbol {
    char name[MAX_NAME_LENGTH]; // Имя символа
    enum SymbolType type;            // Тип символа
    int scope;                  // Уровень области видимости
    int address;                // Адрес или смещение в памяти
    char dataType[MAX_NAME_LENGTH]; // Тип данных (например, int, float)
} Symbol;

// Таблица символов
typedef struct SymbolTable {
    struct Symbol* symbols;  // Массив символов
    size_t size;      // Текущее количество символов
    size_t capacity;  // Максимальная емкость массива
} SymbolTable;

void initSymbolTable(SymbolTable* table);
void addSymbol(SymbolTable* table, const char* name, SymbolType type, int scope, int address, const char* dataType);
Symbol* findSymbol(SymbolTable* table, const char* name);
void freeSymbolTable(SymbolTable* table);