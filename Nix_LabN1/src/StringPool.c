#include "StringPool.h"

// Инициализация
StringPool* init_pool() {
    StringPool* pool = malloc(sizeof(StringPool));
    pool->count = 0;
    return pool;
}

// Добавление строки. Возвращает уникальное имя строки.
const char* intern_string(StringPool* pool, const char* str, bool* isRepeat) {
    for (int i = 0; i < pool->count; ++i) {
        if (strcmp(pool->strings[i], str) == 0) {
            *isRepeat = true;
            return pool->names[i]; // Уже есть — вернуть имя
        }
    }

    if (pool->count >= MAX_STRINGS) {
        fprintf(stderr, "String pool overflow\n");
    }

    // Новая строка — сохранить и сгенерировать имя
    pool->strings[pool->count] = _strdup(str);
    snprintf(pool->names[pool->count], MAX_NAME_LEN, "str_%d", pool->count);
    return pool->names[pool->count++];
}

// Очистка
void free_pool(StringPool* pool) {
    for (int i = 0; i < pool->count; ++i) {
        free(pool->strings[i]);
    }
    pool->count = 0;
}

