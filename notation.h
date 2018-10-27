#pragma once

#include <string>

using namespace std;

/**********************
 * 
 * there are 6 types of notation in C language
 * - Keyword / Reserved word
 * - Identifier
 * - Operator
 * - Bound notation
 * - Literal value
 * - Comment
 * 
**********************/

enum NotationType
{
	// KEY,			 // keyword
	// ID,				 // identifier
	TOKEN,			// token, include keyword and id
	OP,					// operator
	BD,					// bound
	LTR_CHAR,		// literal char
	LTR_STR,		// literal string
	LTR_INT,		// literal int
	LTR_L,			// literal long
	LTR_LL,			// literal long long
	LTR_FLOAT,	// literal float
	LTR_DOUBLE, // literal double
	PRE,// preprocess command
	END					// reach file end
};

enum Keyword
{
	KEY_auto,
	KEY_break,
	KEY_case,
	KEY_char,
	KEY_const,
	KEY_continue,
	KEY_default,
	KEY_do,
	KEY_int,
	KEY_long,
	KEY_register,
	KEY_return,
	KEY_short,
	KEY_signed,
	KEY_sizeof,
	KEY_static,
	KEY_struct,
	KEY_switch,
	KEY_typedef,
	KEY_union,
	KEY_unsigned,
	KEY_void,
	KEY_volatile,
	KEY_while,
	KEY_double,
	KEY_else,
	KEY_enum,
	KEY_extern,
	KEY_float,
	KEY_for,
	KEY_goto,
	KEY_if
};

enum Operator
{
	//====================== relational operators
	REL_EQ, // ==
	REL_GT, // >
	REL_LS, // <
	REL_GE, // >=
	REL_LE, // <=
	REL_NE, // !=

	//====================== arithmatic operators
	MATH_ADD, // +
	MATH_SUB, // -
	MATH_MUL, // *
	MATH_DIV, // /
	MATH_MOD, // %
	MATH_INC, // ++
	MATH_DEC, // --

	//======================== logical operators
	LOG_AND, // &&
	LOG_OR,	// ||
	LOG_NOT, // !

	//========================= bitwise operators
	BIT_AND,	// &
	BIT_OR,		// |
	BIT_XOR,	// ^
	BIT_FLIP, // ~
	BIT_LS,		// <<
	BIT_RS,		// >>

	//========================= assignment operators
	ASN_NORMAL, // =
	ASN_ADD,		// +=
	ASN_SUB,		// -=
	ASN_MUL,		// *=
	ASN_DIV,		// /=
	ASN_MOD,		// %=
	ASN_LS,			// <<=
	ASN_RS,			// >>=
	ASN_AND,		// &=
	ASN_OR,			// |=
	ASN_XOR,		// ^=

	//===========================others
	CON_QUS, // ?
	CON_COL, // :
	MEMBER,	// .
	POINTER // ->
};

enum Bound
{
	L_parenthese, // (
	R_parenthese, // )
	L_bracket,		// [
	R_bracket,		// ]
	L_brace,			// {
	R_brace,			// }
	SEMICOLON,		// ;
	COMMA					// ,
};

string toString(NotationType arg);
string toString(Keyword arg);
string toString(Operator arg);
string toString(Bound arg);