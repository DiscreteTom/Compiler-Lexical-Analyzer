#pragma once

#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Buffer
{
private:
	struct Msg
	{
		enum MsgType
		{
			ERROR,
			WARNING
		};
		MsgType type;
		long long line;
		long long column;
		string msg;
	};

	ifstream fin;
	long long lineCount;
	long long columnCount;
	long long preColumnCount; // column count of the previous line
	long long charCount;			//include comment
	vector<Msg> msgs;					// include warnings and errors

public:
	Buffer(const string &fileName);
	~Buffer();
	char nextChar();
	char nextNbChar();
	void retract();
	void setError(const string &msg);
	void setWarning(const string &msg);
	void toNextLine();
	void showMsg() const;
	void showCounter() const;
};