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
		int line;
		long long column;
		string msg;
	};

	ifstream fin;
	int lineCount;
	long long columnCount;
	long long preColumnCount;
	int charCount; //exclude comment
	vector<Msg> msgs;

public:
	Buffer(const string &fileName);
	char nextChar();
	char nextNbChar();
	void retract();
	void setError(const string &msg);
	void setWarning(const string &msg);
	void toNextLine();
	void showMsg() const ;
};