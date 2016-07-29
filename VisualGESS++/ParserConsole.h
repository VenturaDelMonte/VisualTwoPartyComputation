/*
	file name: ParserConsole.h
	author: Anna De Angelis
	created on: 1/07/13
*/

#pragma once
#ifndef __PARSERCONSOLE_H__
#define __PARSERCONSOLE_H__


class CParserConsole
{
	public:
						CParserConsole						(char **p, int size)	: func(""), fdp(8), exp(16)	{ Parse(p, size);}
		virtual			~CParserConsole						();

		void			Parse								(char **p, int size);
		LPCSTR			getFunction							() const;
		bool			notFoundVariables					(int variable);
		

		string			func;
		DEFINE_ASSOC_VECTOR(VARIABLES, u32, bool);
		VARIABLES		variables_vector;
		string			output;
		s32				fdp, exp;
};

#endif