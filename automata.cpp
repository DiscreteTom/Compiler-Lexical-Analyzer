#include "automata.h"
#include <climits>

Automata::Automata(Buffer &buffer) : buf(buffer)
{
	state = NORMAL;
	num = decimal = 0;
}

void Automata::setError(const string &msg)
{
	buf.setError(msg);
	state = NORMAL;
	num = decimal = 0;
	token = "";
}

NotationType Automata::nextNotationType()
{
	state = NORMAL;
	bool flag = true; //continue loop
	char c = 0;
	num = decimal = 0;
	token = "";
	while (flag && c != EOF)
	{
		switch (state)
		{
		case NORMAL:
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
					currentType = OP;
					currentOperator = PRE;
					return currentType;
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
				case EOF:
					flag = false;
					break;
				default:
					token = "Can't recognize '";
					token += c;
					token += "' in this file";
					setError(token);
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
			break;
		case SINGLE_NOTE:
			while (state == SINGLE_NOTE && c != EOF)
			{
				c = buf.nextChar();
				if (c == '\n')
					state = NORMAL;
			}
			break;
		case TO_BE_NORMAL:
			c = buf.nextChar();
			if (c == '/')
				state = NORMAL;
			else
				state = MULTI_NOTE;
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
				state = HAS_LS;
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
			case '>':
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
				state = IS_DOUBLE_NORMAL;
				point = 0;
				decimal = c - '0';
				decimal /= 10;
			}
			else
			{
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
			}
			buf.retract();
			currentType = NotationType::TOKEN;
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
			else if (c >= 32 && c <= 126)
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
			}
			else
			{
				setError("Can not recognize character");
			}
			break;
		case IS_LTR_STR:
			c = buf.nextChar();
			while (c != EOF && c != '\"' && c != '\\')
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
			}
			if (num > 0)
			{
				if (c == '.')
					state = IS_DOUBLE_NORMAL;
				decimal = num;
				point = 0;
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
			else if (c == '.')
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
			if (c >= '0' && c <= '7')
			{
				num *= 8;
				num += c - '0';
			}
			else if (c == '8' || c == '9')
			{
				setError("Octopus number can NOT contain '8' and '9'");
			}
			else if (c == '.')
			{
				setError("Literal octopus number can NOT be decimal");
			}
			else if (c == 'e')
			{
				setError("Literal octopus number can NOT use exponential expression");
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
			if (c == '0' || c == '1')
			{
				num *= 2;
				num += c - '0';
			}
			else if (c >= '2' && c <= '9')
			{
				setError("Binary number can NOT contain number larger than 1");
			}
			else if (c == '.')
			{
				setError("Literal binary number can NOT be decimal");
			}
			else if (c == 'e')
			{
				setError("Literal binary number can NOT use exponential expression");
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
		case IS_INT_EXP:
			c = buf.nextChar();
			if (c == '+')
			{
				exp = 0;
				state = IS_INT_EXP_P;
			}
			else if (c == '-')
			{
				exp = 0;
				state = IS_INT_EXP_N;
			}
			else if (c >= '0' && c <= '9')
			{
				buf.retract();
				state = IS_INT_EXP_P;
			}
			else
			{
				setError("Wrong char in exponential expression");
			}
			break;
		case IS_INT_EXP_P:
			c = buf.nextChar();
			while (c >= '0' && c <= '9')
			{
				exp *= 10;
				exp += c - '0';
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
					num *= 10;
				}
				if (num < 0)
				{
					setError("Exponential number overflow");
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
		case IS_INT_EXP_N:
			c = buf.nextChar();
			while (c >= '0' && c <= '9')
			{
				exp *= 10;
				exp += c - '0';
			}
			buf.retract();
			decimal = num;
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
		case IS_DOUBLE_NORMAL:
			c = buf.nextChar();
			while (c != EOF && (c >= '0' && c <= '9'))
			{
				double t = c - '0';
				for (int i = 0; i < point; ++i)
				{
					t /= 10;
				}
				decimal += t;
				++point;
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
