#include "notation.h"

string toString(NotationType arg)
{
	switch (arg)
	{
	case KEY:
		return "KEYWORD";
	case ID:
		return "IDENTIFIER";
	case OP:
		return "OPERATOR";
	case BD:
		return "BOUND";
	case LTR_CHAR:
		return "LTR_CHAR";
	case LTR_STR:
		return "LTR_STRING";
	case LTR_INT:
		return "LTR_INT";
	case LTR_L:
		return "LTR_LONG";
	case LTR_LL:
		return "LTR_LONGLONG";
	case LTR_FLOAT:
		return "LTR_FLOAT";
	case LTR_DOUBLE:
		return "LTR_DOUBLE";
	case END:
		return "END";
	case PRE:
		return "PRE";
	default:
		return "";
	}
}

string toString(Keyword arg)
{
	switch (arg)
	{
	case KEY_auto:
		return "auto";
	case KEY_break:
		return "break";
	case KEY_case:
		return "case";
	case KEY_char:
		return "char";
	case KEY_const:
		return "const";
	case KEY_continue:
		return "continue";
	case KEY_default:
		return "default";
	case KEY_do:
		return "do";
	case KEY_int:
		return "int";
	case KEY_long:
		return "long";
	case KEY_register:
		return "register";
	case KEY_return:
		return "return";
	case KEY_short:
		return "short";
	case KEY_signed:
		return "signed";
	case KEY_sizeof:
		return "sizeof";
	case KEY_static:
		return "static";
	case KEY_struct:
		return "struct";
	case KEY_switch:
		return "switch";
	case KEY_typedef:
		return "typedef";
	case KEY_union:
		return "union";
	case KEY_unsigned:
		return "unsigned";
	case KEY_void:
		return "void";
	case KEY_volatile:
		return "volatile";
	case KEY_while:
		return "while";
	case KEY_double:
		return "double";
	case KEY_else:
		return "else";
	case KEY_enum:
		return "enum";
	case KEY_extern:
		return "extern";
	case KEY_float:
		return "float";
	case KEY_for:
		return "for";
	case KEY_goto:
		return "goto";
	case KEY_if:
		return "if";
	default:
		return "";
	}
}

string toString(Operator arg)
{
	switch (arg)
	{
	//====================== relational operators
	case REL_EQ:
		return "==";
	case REL_GT:
		return ">";
	case REL_LS:
		return "<";
	case REL_GE:
		return ">=";
	case REL_LE:
		return "<=";
	case REL_NE:
		return "!=";

	//====================== arithmatic operators
	case MATH_ADD:
		return "+";
	case MATH_SUB:
		return "-";
	case MATH_MUL:
		return "*";
	case MATH_DIV:
		return "/";
	case MATH_MOD:
		return "%";
	case MATH_INC:
		return "++";
	case MATH_DEC:
		return "--";

	//======================== logical operators
	case LOG_AND:
		return "&&";
	case LOG_OR:
		return "||";
	case LOG_NOT:
		return "!";

	//========================= bitwise operators
	case BIT_AND:
		return "&";
	case BIT_OR:
		return "|";
	case BIT_XOR:
		return "^";
	case BIT_FLIP:
		return "~";
	case BIT_LS:
		return "<<";
	case BIT_RS:
		return ">>";

	//========================= assignment operators
	case ASN_NORMAL:
		return "=";
	case ASN_ADD:
		return "+=";
	case ASN_SUB:
		return "-=";
	case ASN_MUL:
		return "*=";
	case ASN_DIV:
		return "/=";
	case ASN_MOD:
		return "%=";
	case ASN_LS:
		return "<<=";
	case ASN_RS:
		return ">>=";
	case ASN_AND:
		return "&=";
	case ASN_OR:
		return "|=";
	case ASN_XOR:
		return "^=";

	//===========================others
	case CON_QUS:
		return "?";
	case CON_COL:
		return ":";
	case MEMBER:
		return ".";
	case POINTER:
		return "->";
	default:
		return "";
	}
}

string toString(Bound arg)
{
	switch (arg)
	{
	case L_parenthese:
		return "(";
	case R_parenthese:
		return ")";
	case L_bracket:
		return "[";
	case R_bracket:
		return "]";
	case L_brace:
		return "{";
	case R_brace:
		return "}";
	case SEMICOLON:
		return ";";
	case COMMA:
		return ",";
	default:
		return "";
	}
}