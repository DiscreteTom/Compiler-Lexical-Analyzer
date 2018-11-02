#include "buffer.h"
#include "automata.h"
#include "displayer.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

void showDeveloper();
void showHelp();

int main(int argc, char **argv)
{
	showDeveloper();
	vector<string> fileNames;
	for (int i = 1; i < argc; ++i)
	{
		//get all file names
		fileNames.push_back(argv[i]);
	}
	if (argc == 1)
	{
		//no fileName parameter
		showHelp();
	}
	for (int i = 0; i < fileNames.size(); ++i)
	{
		cout << "==================== " << fileNames[i] << " =====================\n";
		Buffer buf(fileNames[i]);
		Automata automata(buf);
		Displayer displayer(automata);
		displayer.start();
		buf.showMsg();
		displayer.showCounter();
		buf.showCounter();
	}
	system("pause");
}

void showDeveloper()
{
	cout << "*************************************************************\n"
			 << "                Compiler-Lexical-Analyzer\n"
			 << "                 Written By DiscreteTom\n"
			 << "                   See source code at\n"
			 << "   https://github.com/DiscreteTom/Compiler-Lexical-Analyzer\n"
			 << "*************************************************************\n\n";
}

void showHelp()
{
	cout << "Drag file(s) on this exe to run\n"
			 << "Or use \"exeName [fileName...] [> <outputFileName>] in console\n";
}