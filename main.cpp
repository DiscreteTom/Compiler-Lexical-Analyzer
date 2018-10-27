#include "buffer.h"
#include "automata.h"
#include "displayer.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char ** argv){
	vector<string> fileNames;
	for (int i = 1; i < argc; ++i){
		fileNames.push_back(argv[i]);
	}
	if (argc == 1){
		string str;
		cout << "Please input a file's name:";
		getline(cin, str);
		fileNames.push_back(str);
	}
	for (int i = 0; i < fileNames.size(); ++i){
		Buffer buf(fileNames[i]);
		Automata automata(buf);
		Displayer displayer(automata);
		displayer.start();
		buf.showMsg();
	}
	system("pause");
}