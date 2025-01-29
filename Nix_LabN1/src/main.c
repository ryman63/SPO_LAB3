#include "FlowGraphBuilder.h"
#include "ControlFlowCompiler.h"


int string_exists(const char* str) {
	return str && str[0] != '\0';
}

int main(int argc, char* argv[]) {
	//#ifdef _WIN32
	//	freopen_s(stderr, "NUL", "w", stderr);
	//#else
	//	freopen("/dev/null", "w", stderr);
	//#endif

	Array* files = buildArray(sizeof(SourceFile), 1);

	for (size_t i = 0; i < argc; i++) {
		if (i == 0 || i == argc - 1) continue;
		if(string_exists(argv[i]))
			pushBack(files, GetSrcFile(argv[i]));
		else
			printf(strcat_s("invalid arg ", strlen(argv[i]), argv[i]));
	}

	CallGraphNode* callGraph = analysis(files, argv[argc - 1]);

	compile(callGraph);

	freeArray(&files);

	system("pause");

	return 0;
}