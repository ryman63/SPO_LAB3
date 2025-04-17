#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STRINGS 1024
#define MAX_NAME_LEN 32

typedef struct {
    char* strings[MAX_STRINGS];     // Уникальные строки
    char names[MAX_STRINGS][MAX_NAME_LEN]; // Уникальные имена
    int count;
} StringPool;

// Инициализация пула
StringPool* init_pool();

// Добавление строки в пул
const char* intern_string(StringPool* pool, const char* str, bool* isRepeat);

// Очистка пула
void free_pool(StringPool* pool);
