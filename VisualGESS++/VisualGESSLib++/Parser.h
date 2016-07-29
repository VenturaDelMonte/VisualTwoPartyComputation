/*
	file name: Parser.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 27/03/2013
*/
#pragma once
#ifndef __PARSER_H__
#define __PARSER_H__

#include "PureParser.h"

class CParser : public IPureParser
{	
	public:
								CParser						()	: number_variable(0)		{}
		virtual					~CParser					();

		virtual bool			Parse						(string func);
		
				string			toString					();
		
		virtual	void			BuildTree					(fastdelegate::FastDelegate1<s16, void>& func);

		virtual	u32				getNumberVariables			() const	{return number_variable;}
	private:
		u32				number_variable;

	private:
		CStack<s16>		stk;
	private:
		bool			match_expression			(char *p);
		int				extract_number				(char **p);
};

#endif