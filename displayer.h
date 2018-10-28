#pragma once

#include "automata.h"
#include "counter.h"

class Displayer
{
	Automata &_am;
	Counter strCounter;
	Counter idCounter;
	Counter keywordCounter;

public:
	Displayer(Automata &automata) : _am(automata){};

	void start();
	void showCounter() const;
};