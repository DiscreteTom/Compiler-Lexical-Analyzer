#pragma once

#include "automata.h"

class Displayer
{
	Automata &_am;

public:
	Displayer(Automata &automata) : _am(automata){};

	void start();
};