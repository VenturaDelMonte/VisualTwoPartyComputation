#pragma once
#ifndef __DESCENT_PARSER_H__
#define __DESCENT_PARSER_H__

#include "PureParser.h"
#include "LinkedTree.h"

class CCircuit;

class CDescentParser : public IPureParser
{

public:
							CDescentParser				(CCircuit& c);
	virtual					~CDescentParser				();


	virtual bool			Parse						(string func);
		
	

	virtual	u32				getNumberVariables			() const	{return m_variable_count;}

private:
	u32					m_variable_count;
	CCircuit&			m_circuit;

};

#endif