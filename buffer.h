#pragma once

#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Buffer
{
private:
	struct Error
	{
		int line;
		string msg;
	};

	ifstream fin;
	int lineCount;
	int charCount; //exclude comment
	vector<Error> errs;

public:
	Buffer(const string &fileName);
	char nextChar();
	char nextNbChar();
	void retract(int n = 1);
	void setError(const string &msg);
};