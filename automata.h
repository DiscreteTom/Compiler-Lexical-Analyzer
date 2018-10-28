#pragma once

#include <string>
#include "state.h"
#include "notation.h"
#include "buffer.h"
#include <map>

using namespace std;

class Automata
{
private:
	Buffer &buf;
	State state;
	string token;
	long long num;
	double decimal;
	int point;
	int exp;
	NotationType currentType;
	Keyword currentKeyword;
	Operator currentOperator;
	Bound currentBound;
	map<string, Keyword> keyMap;

	void setError(const string &msg);
	long long octToDec(long long arg);

public:
	Automata(Buffer &buffer);

	NotationType nextNotationType();

	// string getToken() const { return token; }
	string getID() const { return token; }
	Keyword getKeyword() const { return currentKeyword; }
	Operator getOperator() const { return currentOperator; }
	Bound getBound() const { return currentBound; }

	// literal value
	char getChar() const { return token[0]; }
	string getStr() const { return token; }
	int getInt() const { return (int)num; }
	long getLong() const { return (long)num; }
	long long getLongLong() const { return num; }
	float getFLoat() const { return (float)decimal; }
	double getDouble() const { return decimal; }
};