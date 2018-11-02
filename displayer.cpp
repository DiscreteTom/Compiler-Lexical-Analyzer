#include "displayer.h"
#include <iostream>

using namespace std;

void Displayer::start()
{
	NotationType ct; // currentType
	ct = _am.nextNotationType();
	while (ct != END)
	{
		cout << toString(ct) << ": ";
		switch (ct)
		{
		case KEY:
			cout << toString(_am.getKeyword());
			keywordCounter.insert(toString(_am.getKeyword()));
			break;
		case ID:
			cout << _am.getID();
			idCounter.insert(_am.getID());
			break;
		case OP:
			cout << toString(_am.getOperator());
			break;
		case BD:
			cout << toString(_am.getBound());
			break;
		case LTR_CHAR:
			cout << _am.getChar();
			break;
		case LTR_STR:
			cout << _am.getStr();
			strCounter.insert(_am.getStr());
			break;
		case LTR_INT:
			cout << _am.getInt();
			break;
		case LTR_L:
			cout << _am.getLong();
			break;
		case LTR_LL:
			cout << _am.getLongLong();
			break;
		case LTR_FLOAT:
			cout << _am.getFLoat();
			break;
		case LTR_DOUBLE:
			cout << _am.getDouble();
			break;
		case PRE:
			cout << "PRE";
			break;
		default:
			cout << "ERROR";
		}

		cout << endl;
		ct = _am.nextNotationType();
	}
	cout << "END\n";
}

void Displayer::showCounter() const
{
	cout << endl;
	cout << "Keyword occurrences:\n";
	keywordCounter.show();
	cout << endl
			 << "Identifier occurrences:\n";
	idCounter.show();
	cout << endl
			 << "Literal string occurrences:\n";
	strCounter.show();
}