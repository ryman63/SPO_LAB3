#include "ValueType.h"
#include <stdlib.h>

ValueType compareTypes(ValueType type1, ValueType type2) {
	if (type1 == type2) {
		return type1;
	} 
	else {
		if (type1 == TYPE_INT && type2 == TYPE_FLOAT
			|| type1 == TYPE_FLOAT && type2 == TYPE_INT) {
			return TYPE_FLOAT;
		}
		else if ((type1 == TYPE_STRING && (type2 == TYPE_INT || type2 == TYPE_FLOAT))
			|| ((type1 == TYPE_INT || type1 == TYPE_FLOAT) && type2 == TYPE_STRING)) {
			return TYPE_ERROR;
		}
		else return TYPE_ERROR;
	}
}


ValueType typeIdentify(char* value) {
    // �������� �� ���������� ��������
    if (strcmp(value, "true") == 0 || strcmp(value, "false") == 0) {
        return TYPE_BOOL;
    }

    // �������� �� ������ (������ � ����� � ���������)
    size_t len = strlen(value);
    if (len >= 2 && value[0] == '"' && value[len - 1] == '"') {
        return TYPE_STRING;
    }

    // �������� �� ����� � ��������� ������
    char* endPtr = NULL;
    double floatValue = strtod(value, &endPtr);
    if (*endPtr == '\0' && strchr(value, '.') != NULL) {
        return TYPE_FLOAT;
    }

    // �������� �� ����� �����
    int intValue = atoi(value);  // ����������� � ����� �����
    if (*endPtr == '\0') {       // ��������, ��� ����� �������������� ��� ������ ��������
        return TYPE_INT;
    }

    // ���� ������ �� �������
    printf("����������� ���\n");
    // ��������� ������
    return TYPE_ERROR;
}

int getTypeSize(ValueType type)
{
    switch (type)
    {
    case TYPE_INT: return SIZE_OF_INT;
        break;
    case TYPE_FLOAT: return SIZE_OF_FLOAT;
        break;
    case TYPE_BOOL: return SIZE_OF_BOOL;
        break;
    case TYPE_ARRAY: return SIZE_OF_ARRAY;
        break;
    case TYPE_STRING: return SIZE_OF_STRING;
        break;
    }

    return -1;
}


