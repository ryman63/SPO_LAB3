#include "FlowGraphBuilder.h"
#include "ControlFlowCompiler.h"
#include "CodeGenerator.h"

static char* MEMORY_CODE_SECTION_NAME = "code";
static char* USER_SRC_DIRECTORY = "../user/src";

int string_exists(const char* str) {
	return str && str[0] != '\0';
}

void traverseCallGraphBuiltIn(CallGraphNode* root, FILE* fileDesc) {
	if (root == NULL) return;

	ProgramUnit* currUnit = root->unit;
	if (currUnit->isBuiltIn)
		generateBuiltIn(currUnit, fileDesc);

	// ���������� �������� ��� �����
	for (size_t i = 0; i < root->children->size; i++) {
		CallGraphNode* child = getItem(root->children, i);
		traverseCallGraphBuiltIn(child, fileDesc);
	}
}

int main(int argc, char* argv[]) {
	#ifdef _WIN32
		freopen_s(stderr, "NUL", "w", stderr);
	#else
		freopen("/dev/null", "w", stderr);
	#endif

	Array* files = buildArray(sizeof(SourceFile), 1);

	for (size_t i = 0; i < argc; i++) {
		if (i == 0 || i == argc - 1) continue;
		if(string_exists(argv[i]))
			pushBack(files, GetSrcFile(argv[i], USER_SRC_DIRECTORY));
		else
			printf(strcat_s("invalid arg ", strlen(argv[i]), argv[i]));
	}

	Array* astList = buildArray(sizeof(AstNode), files->size);

	CallGraphNode* callGraph = analysis(files, argv[argc - 1], astList);

	Array* genModules = compile(callGraph);

	FILE* fileDesc = fopen("program.asm", "w");

	// generate user funcs
	addSection(MEMORY_CODE_SECTION_NAME, fileDesc);

	for (size_t i = 0; i < genModules->size; i++) {
		Module* currentModule = getItem(genModules, i);
		generate(currentModule, fileDesc);
	}

	// generate builtin funcs
	traverseCallGraphBuiltIn(callGraph, fileDesc);

	printErrors();

	fclose(fileDesc);

	freeArray(&files);

	for (size_t i = 0; i < astList->size; i++) {
		AstNode* rootAst = getItem(astList, i);
		freeAst(&rootAst);
	}

	system("pause");

	return 0;
}

