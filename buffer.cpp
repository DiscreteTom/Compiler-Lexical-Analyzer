#include "buffer.h"

Buffer::Buffer(const string &fileName)
{
	fin.open(fileName);

	lineCount = 1;
	charCount = 0;
}

char Buffer::nextChar()
{
	char result;
	if (!fin.good())
		result = EOF;
	else
		result = fin.get();
	if (result == '\n')
		++lineCount;
	else if (result != EOF)
		++charCount;
	return result;
}

char Buffer::nextNbChar()
{
	char result;
	do
	{
		result = fin.get();
		if (result == '\n')
			++lineCount;
		else if (result != EOF)
			++charCount;
	} while (result != EOF && (result == '\n' || result == ' ' || result == '\t'));
	return result;
}

void Buffer::retract(int n){
	fin.seekg(-n);
}

void Buffer::setError(const string & msg){
	errs.push_back({lineCount, msg});
}