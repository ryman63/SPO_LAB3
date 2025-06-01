#include "myParser.h"

char* extract_line_from_source(const char* source, ANTLR3_UINT32 line_number) {
    ANTLR3_UINT32 current_line = 1;
    const char* p = source;
    const char* line_start = source;

    while (*p != '\0') {
        if (current_line == line_number) {
            line_start = p;
            break;
        }
        if (*p == '\n') {
            current_line++;
        }
        p++;
    }

    // ищем конец строки
    const char* line_end = line_start;
    while (*line_end != '\0' && *line_end != '\n') {
        line_end++;
    }

    // выделяем память под строку
    size_t len = line_end - line_start;
    char* result = (char*)malloc(len + 1);
    if (result == NULL) return NULL;
    strncpy(result, line_start, len);
    result[len] = '\0';

    return result;
}

char* read_file_to_string(const char* filename, size_t* out_size) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return NULL;
    }

    // Переместимся в конец, чтобы узнать размер
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    if (length < 0) {
        perror("Ошибка при определении размера файла");
        fclose(file);
        return NULL;
    }
    rewind(file); // Вернёмся в начало

    // Выделим память под содержимое файла + нуль-терминатор
    char* buffer = (char*)malloc((size_t)length + 1);
    if (buffer == NULL) {
        perror("Ошибка при выделении памяти");
        fclose(file);
        return NULL;
    }

    // Считываем весь файл
    size_t read_bytes = fread(buffer, 1, (size_t)length, file);
    buffer[read_bytes] = '\0'; // Нуль-терминатор

    fclose(file);

    if (out_size != NULL) {
        *out_size = read_bytes;
    }

    return buffer;
}

char* strip_tabs_and_newlines(const char* input) {
    if (input == NULL) return NULL;

    size_t len = strlen(input);
    char* result = (char*)malloc(len + 1); // максимум столько же, сколько было
    if (result == NULL) return NULL;

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] != '\t' && input[i] != '\n' && input[i] != '\r') {
            result[j++] = input[i];
        }
    }

    result[j] = '\0'; // нуль-терминатор
    return result;
}

void fillAst(char* inputFilePath, pANTLR3_BASE_TREE tree, AstNode* root) {
    size_t sizeFile = 0;
    char* content = read_file_to_string(inputFilePath, sizeFile);
    char* lineStr = extract_line_from_source(content, (int)tree->getLine(tree));
    char* clearLineStr = strip_tabs_and_newlines(lineStr);

    free(content);
    free(lineStr);

    AstNode* node = insert
        (root,
        clearLineStr,
        (const char*) tree->getText(tree)->chars, 
        (int)tree->getLine(tree));
    
    size_t childCount = tree->getChildCount(tree);

    for (size_t i = 0; i < childCount; i++) {
        fillAst(inputFilePath ,tree->getChild(tree, i), node);
    }
}

void handleParserError(pANTLR3_BASE_RECOGNIZER recognizer, char* buffer) {
    pANTLR3_EXCEPTION exception = recognizer->state->exception;

    if (exception) {
        snprintf(buffer,1024, "Error on line %d, position %d: %s\n",
            exception->line,
            exception->charPositionInLine,
            (char*)exception->message);

        // Сброс ошибки после обработки
        recognizer->state->exception = NULL;
    }
}

AstNode* parseCustomLang (char* inputFilePath, char* buffErrors) {
    
    pANTLR3_INPUT_STREAM  input;
    pcustom_langLexer lexer;
    pcustom_langParser parser;
    pANTLR3_COMMON_TOKEN_STREAM tokens;

    input = antlr3FileStreamNew((pANTLR3_UINT8)inputFilePath, ANTLR3_ENC_UTF8);
    lexer = custom_langLexerNew(input);
    tokens = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
    parser = custom_langParserNew(tokens);

    custom_langParser_source_return result = parser->source(parser);
    pANTLR3_BASE_TREE tree = result.tree;

    handleParserError(parser->pParser->rec, buffErrors);

    AstNode* root = createNode("source");

    fillAst(inputFilePath ,tree, root);
    //fixAst(root);

    parser->free(parser);
    tokens->free(tokens);
    lexer->free(lexer);
    input->close(input);

    return root;
}

void printAst(AstNode* root, size_t depth)
{
    for (size_t i = 0; i < depth; i++) {
        printf(" ");
    }

    printf(root->token);
    printf("\n");

    depth++;

    for (size_t j = 0; j < root->children->size; j++) {
        printAst(getItem(root->children, j), depth);
    }
}


void fixAst(AstNode* node) {

    Array* storage = buildArray(sizeof(AstNode), 1);
    dfs(node, storage, "RANGE_LIST");

    for (size_t i = 0; i < storage->size; i++) {

        AstNode* item = getItem(storage, i);
        Array* children = item->children;
        size_t dimensions = children->size;

        if (dimensions > 1)
        {
            // сохраняем название массива и удаляем его из ноды SLICE
            AstNode* parent = item->parent;
            AstNode* identifierOfArray = popBack(parent->children);
            
            //проходимся по всем аргументам листа в обратном порядке кроме последнего
            for (int j = children->size - 2; j >= 0; j--) {
                // забираем аргумент
                AstNode* expr = removeAt(children, j);
                
                // создаём недостающие ноды
                AstNode* createSliceNode = createNode("SLICE_EXPR");
                AstNode* createRangeListNode = createNode("RANGE_LIST");

                // делаем привязки
                addChild(createSliceNode, createRangeListNode);
                addChild(createRangeListNode, expr);
                addChild(parent, createSliceNode);

                // на предпоследней итерации привязываем название массива
                if (children->size == 1) {
                    addChild(createSliceNode, identifierOfArray);
                }

                // спускаемся ниже
                parent = createSliceNode;
            }
        }
    }

    freeArray(&storage);
}