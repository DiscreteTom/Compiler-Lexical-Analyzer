#include "buffer.h"

Buffer::Buffer(const string &fileName)
{
	fin.open(fileName);

	lineCount = 1;
	columnCount = 1;
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
	{
		++lineCount;
		columnCount = 1;
	}
	else
	{
		++columnCount;
	}
	if (result != EOF)
		++charCount;
	return result;
}

char Buffer::nextNbChar()
{
	char result = nextChar();
	while (result != EOF && (result == ' ' || result == '\n' || result == '\t'))
	{
		result = nextChar();
	}
	return result;
}

void Buffer::retract()
{
	// can NOT retract across lines
	fin.seekg(-1);
	--charCount;
	--columnCount;
}

void Buffer::setError(const string &msg)
{
	msgs.push_back({Msg::MsgType::ERROR, lineCount, columnCount, msg});
	//to next line
	while (fin.get() != '\n' && !fin.eof())
		;
}

void Buffer::setWarning(const string &msg)
{
	msgs.push_back({Msg::MsgType::WARNING, lineCount, columnCount, msg});
}

void Buffer::toNextLine(){
	char c = nextChar();
	while (c != EOF && c != '\n'){
		c = nextChar();
	}
}