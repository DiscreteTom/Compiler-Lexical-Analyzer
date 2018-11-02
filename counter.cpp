#include "counter.h"
#include <iostream>

using namespace std;

void Counter::insert(const string &str)
{
	auto i = strMap.find(str);
	if (i != strMap.end())
	{
		//find it in strMap
		++count[i->second];
	}
	else
	{
		// not find it, add it
		strMap.insert(make_pair(str, strMap.size()));
		count.push_back(1);
	}
}

void Counter::show() const
{
	if (!strMap.size())
		cout << "NONE";
	for (auto i : strMap)
	{
		cout << '"' << i.first << "\": " << count[i.second] << endl;
	}
	cout << endl;
}