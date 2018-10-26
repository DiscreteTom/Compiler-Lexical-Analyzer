#pragma once

enum State
{
	NORMAL,
	//===================== about note
	// TO_BE_NOTE, // equals to HAS_DIV
	MULTI_NOTE,
	SINGLE_NOTE,
	TO_BE_NORMAL,
	//===================== about operator
	HAS_EQ,	// has '='
	HAS_GT,	// has '>'
	HAS_LE,	// has '<'
	HAS_EM,	// has '!'
	HAS_ADD, // has '+'
	HAS_SUB, // has '-'
	HAS_AND, // has '&'
	HAS_OR,	// has '|'
	HAS_MUL, // has '*'
	HAS_DIV, // has '/'
	HAS_MOD, // has '%'
	HAS_LS,	// has "<<"
	HAS_RS,	// has ">>"
	HAS_XOR, //has '^'
	HAS_DOT, // has '.'
	//===================== about identifier and keyword
	TOKEN,
	//====================== literal char and string
	LTR_CHAR,
	LTR_CHAR_ESC,		// escape character
	LTR_CHAR_1, //has one char
	LTR_STR,
	LTR_STR_ESC,
	//====================== literal number
	INT_NORMAL,
	INT_FIRST_0, // has '0', maybe hex or oct or binary
	INT_HEX,		 // has "0x"
	INT_OCT,		 //has "0o"
	INT_BI,			 // has '0b
	INT_EXP,		 // has 'e'
	INT_EXP_P;	 // has "e+"
	INT_EXP_N,	 // has "e-"
	DOUBLE_NORMAL,
	DOUBLE_EXP,		// has 'e'
	DOUBLE_EXP_P, // has "e+"
	DOUBLE_EXP_N	//has "e-"
};