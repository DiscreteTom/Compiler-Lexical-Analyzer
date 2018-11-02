#include "automata.h"
#include <climits>

Automata::Automata(Buffer &buffer) : buf(buffer)
{
	state = NORMAL;
	num = decimal = 0;

	//init keyMap
	keyMap.insert(make_pair("auto", KEY_auto));
	keyMap.insert(make_pair("break", KEY_break));
	keyMap.insert(make_pair("case", KEY_case));
	keyMap.insert(make_pair("char", KEY_char));
	keyMap.insert(make_pair("const", KEY_const));
	keyMap.insert(make_pair("continue", KEY_continue));
	keyMap.insert(make_pair("default", KEY_default));
	keyMap.insert(make_pair("do", KEY_do));
	keyMap.insert(make_pair("int", KEY_int));
	keyMap.insert(make_pair("long", KEY_long));
	keyMap.insert(make_pair("register", KEY_register));
	keyMap.insert(make_pair("return", KEY_return));
	keyMap.insert(make_pair("short", KEY_short));
	keyMap.insert(make_pair("signed", KEY_signed));
	keyMap.insert(make_pair("sizeof", KEY_sizeof));
	keyMap.insert(make_pair("static", KEY_static));
	keyMap.insert(make_pair("struct", KEY_struct));
	keyMap.insert(make_pair("switch", KEY_switch));
	keyMap.insert(make_pair("typedef", KEY_typedef));
	keyMap.insert(make_pair("union", KEY_union));
	keyMap.insert(make_pair("unsigned", KEY_unsigned));
	keyMap.insert(make_pair("void", KEY_void));
	keyMap.insert(make_pair("volatile", KEY_volatile));
	keyMap.insert(make_pair("while", KEY_while));
	keyMap.insert(make_pair("double", KEY_double));
	keyMap.insert(make_pair("else", KEY_else));
	keyMap.insert(make_pair("enum", KEY_enum));
	keyMap.insert(make_pair("extern", KEY_extern));
	keyMap.insert(make_pair("float", KEY_float));
	keyMap.insert(make_pair("for", KEY_for));
	keyMap.insert(make_pair("goto", KEY_goto));
	keyMap.insert(make_pair("if", KEY_if));
}

void Automata::setError(const string &msg)
{
	buf.setError(msg);
	//reset automata
	state = NORMAL;
	num = decimal = point = exp = 0;
	token = "";
}

long long Automata::octToDec(long long arg)
{
	//if 'e' follows an octopus number like 056e3
	//then parse it as a decimal number instead of a octopus number
	//so we need to re-parse it from octopus to decimal
	long long result = 0;
	long long e = 1;
	while (arg > 0)
	{
		result += (arg % 8) * e;
		e *= 10;
		arg /= 8;
	}
	return result;
}

NotationType Automata::nextNotationType()
{
	state = NORMAL;
	bool flag = true; //continue loop
	char c = 0;				// current char
	num = decimal = point = exp = 0;
	token = "";
	while (flag && c != EOF)
	{
		switch (state)
		{
		case NORMAL:
			c = buf.nextChar();
			if (c == ' ' || c == '\n' || c == '\t')
				c = buf.nextNbChar();
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
			{
				state = IS_TOKEN;
				token = c;
			}
			else if (c >= '1' && c <= '9')
			{
				state = IS_INT_NORMAL;
				num = c - '0';
			}
			else
			{
				switch (c)
				{
				case '0':
					state = IS_INT_FIRST_0;
					break;
				case '=':
					state = HAS_EQ;
					break;
				case '>':
					state = HAS_GT;
					break;
				case '<':
					state = HAS_LE;
					break;
				case '!':
					state = HAS_EM;
					break;
				case '+':
					state = HAS_ADD;
					break;
				case '-':
					state = HAS_SUB;
					break;
				case '&':
					state = HAS_AND;
					break;
				case '|':
					state = HAS_OR;
					break;
				case '*':
					state = HAS_MUL;
					break;
				case '/':
					state = HAS_DIV;
					break;
				case '%':
					state = HAS_MOD;
					break;
				case '^':
					state = HAS_XOR;
					break;
				case '\'':
					state = IS_LTR_CHAR;
					break;
				case '"':
					state = IS_LTR_STR;
					break;
				case '.':
					state = HAS_DOT;
					break;
				case '#':
					if (buf.isPre()) {
						currentType = PRE;
						buf.toNextLine(); //discard this line
						return currentType;
					} else {
						setError("Preprocessing command must be the first char in a line");
					}
					break;
				case '(':
					currentType = BD;
					currentBound = L_parenthese;
					return currentType;
					break;
				case ')':
					currentType = BD;
					currentBound = R_parenthese;
					return currentType;
					break;
				case '[':
					currentType = BD;
					currentBound = L_bracket;
					return currentType;
					break;
				case ']':
					currentType = BD;
					currentBound = R_bracket;
					return currentType;
					break;
				case '{':
					currentType = BD;
					currentBound = L_brace;
					return currentType;
					break;
				case '}':
					currentType = BD;
					currentBound = R_brace;
					return currentType;
					break;
				case ';':
					currentType = BD;
					currentBound = SEMICOLON;
					return currentType;
					break;
				case ',':
					currentType = BD;
					currentBound = COMMA;
					return currentType;
					break;
				case ':':
					currentType = OP;
					currentOperator = CON_COL;
					return currentType;
				case '?':
					currentType = OP;
					currentOperator = CON_QUS;
					return currentType;
				case '~':
					currentType = OP;
					currentOperator = BIT_FLIP;
					return currentType;
				case EOF:
					flag = false;
					break;
				default:
					token = "Can't recognize '";
					token += c;
					token += "' in this file";
					setError(token);
					token = "";
					break;
				}
			}
			break;
		case MULTI_NOTE:
			while (state == MULTI_NOTE && c != EOF)
			{
				c = buf.nextChar();
				if (c == '*')
					state = TO_BE_NORMAL;
			}
			if (c == EOF)
				return END;
			break;
		case SINGLE_NOTE:
			while (state == SINGLE_NOTE && c != EOF)
			{
				c = buf.nextChar();
				if (c == '\n')
					state = NORMAL;
			}
			if (c == EOF)
				return END;
			break;
		case TO_BE_NORMAL:
			c = buf.nextChar();
			if (c == '/')
				state = NORMAL;
			else if (c != EOF)
				state = MULTI_NOTE;
			else // EOF
				return END;
			break;
		case HAS_EQ:
			c = buf.nextChar();
			switch (c)
			{
			case '=':
				currentType = OP;
				currentOperator = REL_EQ;
				state = NORMAL;
				return currentType;
			default:
				currentType = OP;
				currentOperator = ASN_NORMAL;
				state = NORMAL;
				buf.retract();
				return currentType;
			}
			break;
		case HAS_GT:
			c = buf.nextChar();
			switch (c)
			{
			case '=':
				currentType = OP;
				currentOperator = REL_GE;
				state = NORMAL;
				return currentType;
			case '>':
				state = HAS_RS;
				break;
			default:
				currentType = OP;
				currentOperator = REL_GT;
				state = NORMAL;
				buf.retract();
				return currentType;
			}
			break;
		case HAS_LE:
			c = buf.nextChar();
			switch (c)
			{
			case '=':
				currentType = OP;
				currentOperator = REL_LE;
				state = NORMAL;
				return currentType;
			case '<':
				state = HAS_RS;
				break;
			default:
				currentType = OP;
				currentOperator = REL_LS;
				state = NORMAL;
				buf.retract();
				return currentType;
			}
			break;
		case HAS_EM:
			c = buf.nextChar();
			if (c == '=')
			{
				currentType = OP;
				currentOperator = REL_NE;
				state = NORMAL;
				return currentType;
			}
			else
			{
				currentType = OP;
				currentOperator = LOG_NOT;
				buf.retract();
				state = NORMAL;
				return currentType;
			}
			break;
		case HAS_ADD:
			c = buf.nextChar();
			switch (c)
			{
			case '+':
				currentType = OP;
				currentOperator = MATH_INC;
				state = NORMAL;
				return currentType;
			case '=':
				currentType = OP;
				currentOperator = ASN_ADD;
				state = NORMAL;
				return currentType;
			default:
				currentType = OP;
				currentOperator = MATH_ADD;
				state = NORMAL;
				buf.retract();
				return currentType;
			}
			break;
		case HAS_SUB:
			c = buf.nextChar();
			switch (c)
			{
			case '-':
				currentType = OP;
				currentOperator = MATH_DEC;
				state = NORMAL;
				return currentType;
			case '=':
				currentType = OP;
				currentOperator = ASN_SUB;
				state = NORMAL;
				return currentType;
			case '>':
				currentType = OP;
				currentOperator = POINTER;
				state = NORMAL;
				return currentType;
			default:
				currentType = OP;
				currentOperator = MATH_SUB;
				state = NORMAL;
				buf.retract();
				return currentType;
			}
			break;
		case HAS_AND:
			c = buf.nextChar();
			switch (c)
			{
			case '&':
				currentType = OP;
				currentOperator = LOG_AND;
				state = NORMAL;
				return currentType;
			case '=':
				currentType = OP;
				currentOperator = ASN_AND;
				state = NORMAL;
				return currentType;
			default:
				currentType = OP;
				currentOperator = BIT_AND;
				state = NORMAL;
				buf.retract();
				return currentType;
			}
			break;
		case HAS_OR:
			c = buf.nextChar();
			switch (c)
			{
			case '|':
				currentType = OP;
				currentOperator = LOG_OR;
				state = NORMAL;
				return currentType;
			case '=':
				currentType = OP;
				currentOperator = ASN_OR;
				state = NORMAL;
				return currentType;
			default:
				currentType = OP;
				currentOperator = BIT_OR;
				state = NORMAL;
				buf.retract();
				return currentType;
			}
			break;
		case HAS_MUL:
			c = buf.nextChar();
			switch (c)
			{
			case '=':
				currentType = OP;
				currentOperator = ASN_MUL;
				state = NORMAL;
				return currentType;
			default:
				currentType = OP;
				currentOperator = MATH_MUL;
				state = NORMAL;
				buf.retract();
				return currentType;
			}
			break;
		case HAS_DIV:
			c = buf.nextChar();
			switch (c)
			{
			case '/':
				state = SINGLE_NOTE;
				break;
			case '*':
				state = MULTI_NOTE;
				break;
			case '=':
				currentType = OP;
				currentOperator = ASN_DIV;
				state = NORMAL;
				return currentType;
			default:
				currentType = OP;
				currentOperator = MATH_DIV;
				state = NORMAL;
				buf.retract();
				return currentType;
			}
			break;
		case HAS_MOD:
			c = buf.nextChar();
			switch (c)
			{
			case '=':
				currentType = OP;
				currentOperator = ASN_MOD;
				state = NORMAL;
				return currentType;
			default:
				currentType = OP;
				currentOperator = MATH_MOD;
				state = NORMAL;
				buf.retract();
				return currentType;
			}
			break;
		case HAS_LS:
			c = buf.nextChar();
			switch (c)
			{
			case '=':
				currentType = OP;
				currentOperator = ASN_LS;
				state = NORMAL;
				return currentType;
			default:
				currentType = OP;
				currentOperator = BIT_LS;
				state = NORMAL;
				buf.retract();
				return currentType;
			}
			break;
		case HAS_RS:
			c = buf.nextChar();
			switch (c)
			{
			case '=':
				currentType = OP;
				currentOperator = ASN_RS;
				state = NORMAL;
				return currentType;
			default:
				currentType = OP;
				currentOperator = BIT_RS;
				state = NORMAL;
				buf.retract();
				return currentType;
			}
			break;
		case HAS_XOR:
			c = buf.nextChar();
			switch (c)
			{
			case '=':
				currentType = OP;
				currentOperator = ASN_XOR;
				state = NORMAL;
				return currentType;
			default:
				currentType = OP;
				currentOperator = BIT_XOR;
				state = NORMAL;
				buf.retract();
				return currentType;
			}
			break;
		case HAS_DOT:
			c = buf.nextChar();
			if (c >= '0' && c <= '9')
			{
				buf.retract();
				state = IS_DOUBLE_NORMAL;
			}
			else
			{
				buf.retract();
				state = NORMAL;
				currentType = OP;
				currentOperator = MEMBER;
				return currentType;
			}
			break;
		case IS_TOKEN:
			c = buf.nextChar();
			while ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
			{
				token += c;
				c = buf.nextChar();
			}
			buf.retract();
			// currentType = NotationType::TOKEN;
			{
				auto i = keyMap.find(token);
				if (i != keyMap.end())
				{
					currentType = KEY;
					currentKeyword = i->second;
				}
				else
				{
					currentType = ID;
				}
			}
			return currentType;
			break;
		case IS_LTR_CHAR:
			c = buf.nextChar();
			if (c == '\'')
			{
				setError("no character here");
			}
			else if (c == '\\')
			{
				state = IS_LTR_CHAR_ESC;
			}
			else if (c >= 32 && c <= 126)//valid char in ASCII
			{
				state = IS_LTR_CHAR_1;
				token = c;
			}
			else
			{
				setError("Can not recognize character");
			}
			break;
		case IS_LTR_CHAR_ESC:
			c = buf.nextChar();
			switch (c)
			{
			case 'a':
				token = '\a';
				state = IS_LTR_CHAR_1;
				break;
			case 'b':
				token = '\b';
				state = IS_LTR_CHAR_1;
				break;
			case 'f':
				token = '\f';
				state = IS_LTR_CHAR_1;
				break;
			case 'n':
				token = '\n';
				state = IS_LTR_CHAR_1;
				break;
			case 'r':
				token = '\r';
				state = IS_LTR_CHAR_1;
				break;
			case 't':
				token = '\t';
				state = IS_LTR_CHAR_1;
				break;
			case 'v':
				token = '\v';
				state = IS_LTR_CHAR_1;
				break;
			case '\\':
				token = '\\';
				state = IS_LTR_CHAR_1;
				break;
			case '\'':
				token = '\'';
				state = IS_LTR_CHAR_1;
				break;
			case '"':
				token = '\"';
				state = IS_LTR_CHAR_1;
				break;
			case '0':
				token = '\0';
				state = IS_LTR_CHAR_1;
				break;
			default:
				setError("Can not recognize this escaped char");
				break;
			}
			break;
		case IS_LTR_CHAR_1:
			c = buf.nextChar();
			if (c == '\'')
			{
				state = NORMAL;
				currentType = NotationType::LTR_CHAR;
				return currentType;
			}
			else if (c == '\\')
			{
				buf.setWarning("multi-character character constant");
				state = IS_LTR_CHAR_ESC;
			}
			else if (c >= 32 && c <= 126)
			{
				buf.setWarning("multi-character character constant");
				token = c;
			}
			else
			{
				setError("Can not recognize character");
			}
			break;
		case IS_LTR_STR:
			c = buf.nextChar();
			while (c != EOF && c >= 32 && c <= 126 && c != '\"' && c != '\\' && c != '\n')//valid char in ASCII
			{
				token += c;
				c = buf.nextChar();
			}
			if (c == '\"')
			{
				state = NORMAL;
				currentType = LTR_STR;
				return currentType;
			}
			else if (c == '\\')
			{
				state = IS_LTR_STR_ESC;
			}
			else
			{
				buf.retract();//maybe retract to the previous line when c == '\n'
				setError("Wrong format for literal string");
			}
			break;
		case IS_LTR_STR_ESC:
			c = buf.nextChar();
			switch (c)
			{
			case 'a':
				token += '\a';
				state = IS_LTR_STR;
				break;
			case 'b':
				token += '\b';
				state = IS_LTR_STR;
				break;
			case 'f':
				token += '\f';
				state = IS_LTR_STR;
				break;
			case 'n':
				token += '\n';
				state = IS_LTR_STR;
				break;
			case 'r':
				token += '\r';
				state = IS_LTR_STR;
				break;
			case 't':
				token += '\t';
				state = IS_LTR_STR;
				break;
			case 'v':
				token += '\v';
				state = IS_LTR_STR;
				break;
			case '\\':
				token += '\\';
				state = IS_LTR_STR;
				break;
			case '\'':
				token += '\'';
				state = IS_LTR_STR;
				break;
			case '"':
				token += '\"';
				state = IS_LTR_STR;
				break;
			case '0':
				token += '\0';
				state = IS_LTR_STR;
				break;
			default:
				setError("Can not recognize this escaped char");
				break;
			}
			break;
		case IS_INT_NORMAL:
			c = buf.nextChar();
			while (c >= '0' && c <= '9')
			{
				num *= 10;
				num += c - '0';
				if (num < 0)
				{
					//overflow
					setError("Literal number overflow!");
					break;
				}
				c = buf.nextChar();
			}
			if (num > 0)
			{
				if (c == '.')
				{
					state = IS_DOUBLE_NORMAL;
					decimal = num;
					point = 0;
				}
				else if (c == 'e')
				{
					decimal = num;
					state = IS_DOUBLE_EXP;
				}
				else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
				{
					setError("Token can NOT start with number");
				}
				else
				{
					buf.retract();
					state = NORMAL;
					if (num < INT_MAX)
					{
						currentType = LTR_INT;
					}
					else if (num < LONG_MAX)
					{
						currentType = LTR_L;
					}
					else
					{
						currentType = LTR_LL;
					}
					return currentType;
				}
			}
			break;
		case IS_INT_FIRST_0:
			c = buf.nextChar();
			if (c == 'b')
			{
				state = IS_INT_BI;
			}
			else if (c == 'x')
			{
				state = IS_INT_HEX;
			}
			else if (c >= '0' && c <= '7')
			{
				buf.retract();
				state = IS_INT_OCT;
			}
			else if (c == '8' || c == '9')
			{
				setError("Literal Octopus number can NOT contain '8' and '9'");
			}
			else if (c == '.')
			{
				state = IS_DOUBLE_NORMAL;
				decimal = 0;
			}
			else
			{
				buf.retract();
				num = 0;
				currentType = LTR_INT;
				return currentType;
			}
			break;
		case IS_INT_HEX:
			c = buf.nextChar();
			while ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')){
				if (c >= '0' && c <= '9')
				{
					num *= 16;
					num += c - '0';
				}
				else if (c >= 'A' && c <= 'F')
				{
					num *= 16;
					num += c - 'A' + 10;
				}
				else if (c >= 'a' && c <= 'f')
				{
					num *= 16;
					num += c - 'z' + 10;
				}
				if (num < 0){
					break;
				}
				c = buf.nextChar();
			}
			if (c == '.')
			{
				setError("Literal hexical number can NOT be decimal");
			}
			else if (c == 'e')
			{
				setError("Literal hexical number can NOT use exponential expression");
			}
			else if ((c >= 'g' && c <= 'z') || (c >= 'G' && c <= 'Z') || c == '_')
			{
				setError("Token can NOT start with literal hexical number");
			}
			else
			{
				buf.retract();
				if (num < 0)
				{
					//overflow
					setError("Literal hexical number overflow");
				}
				else
				{
					state = NORMAL;
					if (num < INT_MAX)
					{
						currentType = LTR_INT;
					}
					else if (num < LONG_MAX)
					{
						currentType = LTR_L;
					}
					else
					{
						currentType = LTR_LL;
					}
					return currentType;
				}
			}
			break;
		case IS_INT_OCT:
			c = buf.nextChar();
			while (c >= '0' && c <= '7')
			{
				num *= 8;
				num += c - '0';
				c = buf.nextChar();
			}
			if (c == '8' || c == '9')
			{
				setError("Octopus number can NOT contain '8' and '9'");
			}
			else if (c == '.')
			{
				setError("Literal octopus number can NOT be decimal");
			}
			else if (c == 'e')
			{
				decimal = octToDec(num);
				state = IS_DOUBLE_EXP;
			}
			else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
			{
				setError("Token can NOT start with literal octopus number");
			}
			else
			{
				buf.retract();
				if (num < 0)
				{
					//overflow
					setError("Literal octopus number overflow");
				}
				else
				{
					state = NORMAL;
					if (num < INT_MAX)
					{
						currentType = LTR_INT;
					}
					else if (num < LONG_MAX)
					{
						currentType = LTR_L;
					}
					else
					{
						currentType = LTR_LL;
					}
					return currentType;
				}
			}
			break;
		case IS_INT_BI:
			c = buf.nextChar();
			while (c == '0' || c == '1')
			{
				num *= 2;
				num += c - '0';
				c = buf.nextChar();
			}
			if (c >= '2' && c <= '9')
			{
				setError("Binary number can NOT contain number larger than 1");
			}
			else if (c == '.')
			{
				setError("Literal binary number can NOT be decimal");
			}
			else if (c == 'e')
			{
				decimal = num;
				state = IS_DOUBLE_EXP;
			}
			else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
			{
				setError("Token can NOT start with literal binary number");
			}
			else
			{
				buf.retract();
				if (num < 0)
				{
					//overflow
					setError("Literal binary number overflow");
				}
				else
				{
					state = NORMAL;
					if (num < INT_MAX)
					{
						currentType = LTR_INT;
					}
					else if (num < LONG_MAX)
					{
						currentType = LTR_L;
					}
					else
					{
						currentType = LTR_LL;
					}
					return currentType;
				}
			}
			break;
		case IS_DOUBLE_NORMAL:
			c = buf.nextChar();
			while (c != EOF && (c >= '0' && c <= '9'))
			{
				double t = c - '0';
				for (int i = 0; i <= point; ++i)
				{
					t /= 10;
				}
				decimal += t;
				++point;
				c = buf.nextChar();
			}
			if (c == 'e')
			{
				state = IS_DOUBLE_EXP;
			}
			else
			{
				buf.retract();
				state = NORMAL;
				if (decimal < FLT_MAX)
					currentType = LTR_FLOAT;
				else
					currentType = LTR_DOUBLE;
				return currentType;
			}
			break;
		case IS_DOUBLE_EXP:
			c = buf.nextChar();
			exp = 0;
			if (c == '+')
			{
				state = IS_DOUBLE_EXP_P;
			}
			else if (c >= '0' && c <= '9')
			{
				buf.retract();
				state = IS_DOUBLE_EXP_P;
			}
			else if (c == '-')
			{
				state = IS_DOUBLE_EXP_N;
			}
			else
			{
				setError("Wrong format for literal number");
			}
			break;
		case IS_DOUBLE_EXP_P:
			c = buf.nextChar();
			while (c >= '0' && c <= '9')
			{
				exp *= 10;
				exp += c - '0';
				c = buf.nextChar();
			}
			buf.retract();
			if (exp < 0)
			{
				setError("Exponential number overflow");
			}
			else
			{
				while (exp)
				{
					--exp;
					decimal *= 10;
				}
				if (decimal < 0)
				{
					setError("Exponential number overflow");
				}
				else
				{
					state = NORMAL;
					if (decimal < FLT_MAX)
					{
						currentType = LTR_FLOAT;
					}
					else
					{
						currentType = LTR_DOUBLE;
					}
					return currentType;
				}
			}
			break;
		case IS_DOUBLE_EXP_N:
			c = buf.nextChar();
			while (c >= '0' && c <= '9')
			{
				exp *= 10;
				exp += c - '0';
				c = buf.nextChar();
			}
			buf.retract();
			if (exp < 0)
			{
				setError("Exponential number overflow");
			}
			else
			{
				while (exp)
				{
					--exp;
					decimal /= 10;
				}
				if (decimal < 0)
				{
					setError("Exponential number overflow");
				}
				else
				{
					state = NORMAL;
					if (decimal < FLT_MAX)
					{
						currentType = LTR_FLOAT;
					}
					else
					{
						currentType = LTR_DOUBLE;
					}
					return currentType;
				}
			}
			break;
		}
	}
	// reach EOF
	return END;
}
