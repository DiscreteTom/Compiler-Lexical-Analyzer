#include "buffer.h"
#include <iostream>

Buffer::Buffer(const string &fileName)
{
	fin.open(fileName);

	lineCount = 1;
	columnCount = 1;
	charCount = 0;
	thisLineHasNbChar = false;
	thisIsTheFirstNbChar = false;
}

Buffer::~Buffer()
{
	fin.close();
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
		thisIsTheFirstNbChar = false;
		thisLineHasNbChar = false;
		++lineCount;
		preColumnCount = columnCount;
		columnCount = 1;
	}
	else
	{
		++columnCount;
	}
	if (result != '\n' && result != ' ' && result != '\t') {
		if (!thisLineHasNbChar) {
			thisLineHasNbChar = true;
			thisIsTheFirstNbChar = true;
		} else {
			thisIsTheFirstNbChar = false;
		}
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
	if (!fin.eof())
		--charCount;
	fin.seekg(-1, ios::cur);
	--columnCount;
	if (columnCount == 0)
	{
		//retract to a previous line
		--lineCount;
		columnCount = preColumnCount;
	}
}

void Buffer::setError(const string &msg)
{
	msgs.push_back({Msg::MsgType::ERROR, lineCount, columnCount, msg});
	// discard this line
	toNextLine();
}

void Buffer::setWarning(const string &msg)
{
	msgs.push_back({Msg::MsgType::WARNING, lineCount, columnCount, msg});
}

void Buffer::toNextLine()
{
	char c = nextChar();
	while (c != EOF && c != '\n')
	{
		c = nextChar();
	}
}

bool Buffer::isPre()
{
	if (thisLineHasNbChar && thisIsTheFirstNbChar)return true;
	return false;
}

void Buffer::showMsg() const
{
	for (int i = 0; i < msgs.size(); ++i)
	{
		if (msgs[i].type == Msg::MsgType::WARNING)
		{
			cout << "Warning";
		}
		else
		{
			cout << "Error";
		}
		cout << " - Line" << msgs[i].line << " - Column" << msgs[i].column << ": " << msgs[i].msg << endl;
	}
}

void Buffer::showCounter() const
{
	cout << endl;
	cout << "Line count: " << lineCount << endl;
	cout << "Character count: " << charCount << endl
			 << endl;
}