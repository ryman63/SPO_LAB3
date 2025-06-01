#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ValueType.h"

// Максимальная длина имени символа
#define MAX_NAME_LENGTH 64

// Типы символов
typedef enum SymbolType {
    SYMBOL_VARIABLE,   // Переменная
    SYMBOL_FUNC_PARAM,   // Переменная
    SYMBOL_FUNCTION,   // Функция
    SYMBOL_CONSTANT    // Константа
} SymbolType;

typedef enum SymbolLocation {
    LOC_REG,
    LOC_STACK,
    LOC_HEAP
};

// Информация о символе
typedef struct Symbol {
    char name[MAX_NAME_LENGTH]; // Имя символа
    enum SymbolType type;            // Тип символа
//    int scope;                  // Уровень области видимости
    int address;                // Адрес или смещение в памяти
    ValueType* valueType; // Тип данных (например, int, float)
    enum SymbolLocation location;
} Symbol;

// Таблица символов
typedef struct SymbolTable {
    struct Symbol* symbols;  // Массив символов
    size_t size;      // Текущее количество символов
    size_t capacity;  // Максимальная емкость массива
} SymbolTable;

void initSymbolTable(SymbolTable* table);
void addSymbol(SymbolTable* table, const char* name, SymbolType type, int address, ValueType* valueType);
Symbol* findSymbol(SymbolTable* table, const char* name);
void freeSymbolTable(SymbolTable* table);