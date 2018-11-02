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
	IS_TOKEN,
	//====================== literal char and string
	IS_LTR_CHAR,
	IS_LTR_CHAR_ESC, // escape character
	IS_LTR_CHAR_1,	 //has one char
	IS_LTR_STR,
	IS_LTR_STR_ESC,
	//====================== literal number
	IS_INT_NORMAL,
	IS_INT_FIRST_0, // has '0', maybe hex or oct or binary
	IS_INT_HEX,			// has "0x"
	IS_INT_OCT,			//has "0o"
	IS_INT_BI,			// has '0b
	IS_DOUBLE_NORMAL,
	IS_DOUBLE_EXP,	 // has 'e'
	IS_DOUBLE_EXP_P, // has "e+"
	IS_DOUBLE_EXP_N	//has "e-"
};