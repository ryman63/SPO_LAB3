#include "ValueType.h"
#include <stdlib.h>

ValueType* compareTypes(ValueType* left, ValueType* right, AstNode* astNode) {
    if (left->kind != right->kind) {
        collectError(ST_TYPING, "incompatible kind of types", astNode->token, astNode->line);
        return NULL;
    }

    switch (left->kind)
    {
    case TYPE_KIND_CUSTOM: {
        if (strcmp(left->custom.name, right->custom.name) == 0
            && left->custom.size == right->custom.size) {
            return left;
        }
    } break;
    case TYPE_KIND_BUILTIN: {
        BuiltInType type1 = left->builtin.type;
        BuiltInType type2 = right->builtin.type;
        if (type1 == type2) return left;
        //else if (type1 == TYPE_INT && type2 == TYPE_FLOAT
        //    || type1 == TYPE_FLOAT && type2 == TYPE_INT) {
        //    return TYPE_FLOAT;
        //}
        //else return TYPE_ERROR;
    } break;
    case TYPE_KIND_ARRAY: {
        if(left->array.size == right->array.size) 
            return compareTypes(left->array.of, right->array.of, astNode);
    }  break;
    case TYPE_KIND_POINTER: {
        return compareTypes(left->pointer.to, right->pointer.to, astNode);
    } break;
    }

    return NULL;
}

ValueType* tryCoerce(ValueType* left, ValueType* right)
{
    return NULL;
}

ValueType* typeIdentify(char* value, AstNode* astNode) {
    ValueType* resultType = NULL;

    // Проверка на логическое значение
    if (strcmp(value, "true") == 0 || strcmp(value, "false") == 0) {
        return createBuiltInType(TYPE_BOOL, astNode);
    }

    // Проверка на константную строку (начало и конец с кавычками), объявление: error = "error";
    size_t len = strlen(value);
    if (len >= 2 && value[0] == '"' && value[len - 1] == '"') {
        return createBuiltInPtr(TYPE_CHAR, astNode);
    }
    // Проверка на массив символов, объявление: str = "";
    len = strlen(value);
    if (len = 2 && value[0] == '"' && value[len - 1] == '"') {
        return createBuiltInArray(TYPE_CHAR, 0, astNode);
    }

    // Проверка на символ (начало и конец с апострофами)
    len = strlen(value);
    if (len >= 2 && value[0] == '\'' && value[len - 1] == '\'') {
        return createBuiltInType(TYPE_CHAR, astNode);
    }

    // Проверка на число с плавающей точкой
    char* endPtr = NULL;
    double floatValue = strtod(value, &endPtr);
    if (*endPtr == '\0' && strchr(value, '.') != NULL) {
        return createBuiltInType(TYPE_FLOAT, astNode);
    }

    // Проверка на целое число
    int intValue = atoi(value);  // Преобразуем в целое число
    if (*endPtr == '\0') {       // Проверка, что после преобразования нет лишних символов
        return createBuiltInType(TYPE_INT, astNode);
    }

    // Если ничего не подошло
    collectError(ST_TYPING, "unknown type", astNode->token, astNode->line);
    // обработка ошибки
    return NULL;
}

int getBuiltInTypeSize(BuiltInType type)
{
    switch (type)
    {
    case TYPE_INT: return SIZE_OF_INT;
        break;
    case TYPE_FLOAT: return SIZE_OF_FLOAT;
        break;
    case TYPE_BOOL: return SIZE_OF_BOOL;
        break;
    case TYPE_CHAR: return SIZE_OF_CHAR;
        break;
    case TYPE_VOID: return SIZE_OF_VOID;
    }

    return -1;
}

ValueType* createBuiltInType(enum BuiltInType type, AstNode* astNode)
{
    ValueType* valueType = malloc(sizeof(ValueType));
    valueType->kind = TYPE_KIND_BUILTIN;
    valueType->builtin.type = type;

    int typeSize = getBuiltInTypeSize(type);
    if (typeSize < 0) {
        collectError(ST_TYPING, "unknown type", astNode->token, astNode->line);
    }
    valueType->builtin.size = typeSize;

    return valueType;
}

ValueType* createBuiltInArray(enum BuiltInType srcType, size_t size, AstNode* astNode)
{
    ValueType* valueTypeArray = malloc(sizeof(ValueType));
    valueTypeArray->kind = TYPE_KIND_ARRAY;

    ValueType* srcValueType = malloc(sizeof(ValueType));
    srcValueType->kind = TYPE_KIND_BUILTIN;
    srcValueType->builtin.type = srcType;

    int typeSize = getBuiltInTypeSize(srcType);
    if (typeSize < 0) {
        collectError(ST_TYPING, "unknown type", astNode->token, astNode->line);
    }
    srcValueType->builtin.size = typeSize;

    valueTypeArray->array.of = srcValueType;

    if (size < 0) {
        collectError(ST_TYPING, "incorrect array size", astNode->token, astNode->line);
    }
    valueTypeArray->array.size = size;

    return valueTypeArray;
}

BuiltInType getBuiltInTypeFromString(char* typeStr)
{
    if (!strcmp(typeStr, "int")) {
        return TYPE_INT;
    }
    else if (!strcmp(typeStr, "float")) {
        return TYPE_FLOAT;
    }
    else if (!strcmp(typeStr, "bool")) {
        return TYPE_BOOL;
    }
    else if (!strcmp(typeStr, "char")) {
        return TYPE_CHAR;
    }
    else if (!strcmp(typeStr, "byte")) {
        return TYPE_BYTE;
    }
    else if (!strcmp(typeStr, "void")) {
        return TYPE_VOID;
    }
    else return TYPE_ERROR;
}

TypeKind getTypeKindFromString(char* typeKindStr)
{
    if (!strcmp(typeKindStr, "BUILTIN")) {
        return TYPE_KIND_BUILTIN;
    }
    else if (!strcmp(typeKindStr, "ARRAY")) {
        return TYPE_KIND_ARRAY;
    }
    else if (!strcmp(typeKindStr, "CUSTOM")) {
        return TYPE_KIND_CUSTOM;
    }
    else if (!strcmp(typeKindStr, "POINTER")) {
        return TYPE_KIND_POINTER;
    }
}

bool isConstString(ValueType* vType) {
    if (vType->kind == TYPE_KIND_POINTER) {
        ValueType* srcTypeOfPtr = vType->pointer.to;
        if (srcTypeOfPtr->kind == TYPE_KIND_BUILTIN && srcTypeOfPtr->builtin.type == TYPE_CHAR) {
            return true;
        }
    }
    return false;
}

size_t getSizeOfArray(ValueType* vType)
{
    size_t itemSize = 0;
    size_t arrayLen = vType->array.size;
    ValueType* srcVType = vType->array.of;

    return arrayLen * getSizeOfType(srcVType);
}

size_t getSizeOfType(ValueType* vType)
{
    size_t sizeOf = 0;

    switch (vType->kind)
    {
    case TYPE_KIND_BUILTIN: {
        sizeOf = vType->builtin.size;
    } break;
    case TYPE_KIND_ARRAY: {
        sizeOf = getSizeOfArray(vType);
    } break;
    case TYPE_KIND_POINTER: {
        sizeOf = SIZE_OF_PTR;
    } break;
    case TYPE_KIND_CUSTOM: {
        sizeOf = vType->custom.size;
    } break;
    default:
        break;
    }

    return sizeOf;
}

ValueType* createBuiltInPtr(enum BuiltInType srcType, AstNode* astNode)
{
    ValueType* valueTypePtr = malloc(sizeof(ValueType));
    valueTypePtr->kind = TYPE_KIND_POINTER;

    ValueType* srcValueType = malloc(sizeof(ValueType));
    srcValueType->kind = TYPE_KIND_BUILTIN;
    srcValueType->builtin.type = srcType;

    int typeSize = getBuiltInTypeSize(srcType);
    if (typeSize <= 0) {
        collectError(ST_TYPING, "unknown type", astNode->token, astNode->line);
    }
    srcValueType->builtin.size = typeSize;

    valueTypePtr->pointer.to = srcValueType;

    return valueTypePtr;
}


