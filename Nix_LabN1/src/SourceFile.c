#include "SourceFile.h"

SourceFile* GetSrcFile(char* fileName, char* directoryPath) {
	SourceFile* srcFile = malloc(sizeof(SourceFile));
	srcFile->fileName = fileName;
	srcFile->directoryPath = directoryPath;
	srcFile->id = id_counter;
	id_counter++;
	return srcFile;
}