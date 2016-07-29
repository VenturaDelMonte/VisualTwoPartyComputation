/*
	file name: Parser.cpp
	author: Ventura Del Monte & Anna De Angelis
	created on: 27/03/2013
*/

#include "stdafx.h"
#include "Parser.h"
#include "AvailableOperations.h"

#define OR_SYMBOL '+'

CParser::~CParser()
{
	stk.clear();
}

bool CParser::Parse(string func)
{
	const char* ptr = *func;
	Log("parsing %s", ptr);
	char* p = const_cast<char*>(ptr);
	return match_expression(p);
}

#define IS_NUM(x) (x >= '0' && x <= '9')
#define POP(S)	S.top(); S.pop();

/**
	Return true if match expression, false otherwise
*/
bool CParser::match_expression(char *p)
{
	CStack<s16> S;
	s16 matched = 0;
	u32 mask = 0, test_mask = 0;
	u16 max_x = 0;
	while (*p)
	{
		switch (*p)
		{
			case 32:
			case 9:
			case 13:
			{
				++p;
				break;
			}
			case '(':
			{
				S.push(LEFT_PARENTHESIS);
				++p;
				matched++;
				break;
			}
			case ')':
			{
				while (!S.empty() && S.top() != LEFT_PARENTHESIS)
				{
					s16 e = POP(S);
					stk.push(e);
				}
				if (S.empty())
					return false;
				S.pop();
				matched--;
				++p;
				break;
			}
			case '+':
			case '*':
			{
				u16 op = (*p == OR_SYMBOL) ? OR_OPERATION : AND_OPERATION;
				if (S.empty() || S.top() == LEFT_PARENTHESIS)
				{
					S.push(op);
				}
				else
				{
					while (!S.empty() && S.top() != LEFT_PARENTHESIS)
					{
						s16 e = POP(S);
						stk.push(e);
					}
					if (S.empty())
						return false;
					S.push(op);
				}
				++p;
				break;
			}
			default:
			{
				int n = -1;
				if (*p == 'x' && IS_NUM(*(p + 1)))
				{
					n = extract_number(&(++p));
					if (n != -1)
					{	
						u32 x = (1 << (n - 1));
						if (mask & x)
							return false;
						stk.push(n);
						max_x = (n > max_x) ? n : max_x;
						mask |= x;
						number_variable++;

					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
				break;
			}
		}
	}
	while (!S.empty())
	{
		s16 e = POP(S);	
		if (e == LEFT_PARENTHESIS)
			continue;
		stk.push(e);		
	}
	for (u16 i = 0; i < max_x; i++)
		test_mask |= (1 << i);
	return matched == 0 && !(mask ^ test_mask);
}

/**
	Return the number associated to **p
*/
int CParser::extract_number(char **p)
{
	int ret = 0;
	string str = "";
	while (**p && IS_NUM(**p))
	{
		str += **p;
		++(*p);
	}
	return atoi(*str);
}

string CParser::toString()
{
	CStack<s16> S;
	string ret = "";
	while (stk.size())
	{
		s16 e = stk.top();
		ret += format("%d ", e);
		S.push(e);
		stk.pop();
	}
	while (!S.empty())
	{
		stk.push(S.top());
		S.pop();
	}
	return ret;
}

/**
	It calls the callback from the circuit in order to build tree following the post-fix notation in the stack
*/
void CParser::BuildTree(fastdelegate::FastDelegate1<s16, void>& func)
{
	CVector<s16> queue;
	while (stk.size())
	{
		int e = POP(stk);
		switch (e)
		{
			case OR_OPERATION:
			case AND_OPERATION:
			{
				func(e);
				break;
			}
			default:
			{
				queue.push_back(e);
				break;
			}
		}
	}
	while (queue.size())
	{
		func(queue.front());
		queue.erase(queue.begin());
	}
	queue.clear();
}