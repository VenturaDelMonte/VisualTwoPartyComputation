#pragma once
#ifndef __PURE_PARSER_H__
#define __PURE_PARSER_H__

class ABSTRACT IPureParser
{
public:

	typedef fastdelegate::FastDelegate1<s16, void>		callback;

	virtual					~IPureParser				()												{ }
	virtual bool			Parse						(string func)									= 0;
	
	virtual u32				getNumberVariables			() const										= 0;

};

#endif