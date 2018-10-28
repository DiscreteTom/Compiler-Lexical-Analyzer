#pragma once

#include <map>
#include <string>
#include <vector>

using namespace std;

class Counter
{
private:
	map<string, int> strMap;
	vector<int> count;

public:
	void insert(const string &str);
	void show() const;
};