#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STRINGS 1024
#define MAX_NAME_LEN 32

typedef struct {
    char* strings[MAX_STRINGS];     // ���������� ������
    char names[MAX_STRINGS][MAX_NAME_LEN]; // ���������� �����
    int count;
} StringPool;

// ������������� ����
StringPool* init_pool();

// ���������� ������ � ���
const char* intern_string(StringPool* pool, const char* str, bool* isRepeat);

// ������� ����
void free_pool(StringPool* pool);
