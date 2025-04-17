#include "StringPool.h"

// �������������
StringPool* init_pool() {
    StringPool* pool = malloc(sizeof(StringPool));
    pool->count = 0;
    return pool;
}

// ���������� ������. ���������� ���������� ��� ������.
const char* intern_string(StringPool* pool, const char* str, bool* isRepeat) {
    for (int i = 0; i < pool->count; ++i) {
        if (strcmp(pool->strings[i], str) == 0) {
            *isRepeat = true;
            return pool->names[i]; // ��� ���� � ������� ���
        }
    }

    if (pool->count >= MAX_STRINGS) {
        fprintf(stderr, "String pool overflow\n");
    }

    // ����� ������ � ��������� � ������������� ���
    pool->strings[pool->count] = _strdup(str);
    snprintf(pool->names[pool->count], MAX_NAME_LEN, "str_%d", pool->count);
    return pool->names[pool->count++];
}

// �������
void free_pool(StringPool* pool) {
    for (int i = 0; i < pool->count; ++i) {
        free(pool->strings[i]);
    }
    pool->count = 0;
}

