#include "counter.h"
#include <iostream>

using namespace std;

void Counter::insert(const string &str)
{
	auto i = strMap.find(str);
	if (i != strMap.end())
	{
		++count[i->second];
	}
	else
	{
		strMap.insert(make_pair(str, strMap.size()));
		count.push_back(1);
	}
}

void Counter::show() const
{
	for (auto i : strMap)
	{
		cout << i.first << ": " << count[i.second] << endl;
	}
}