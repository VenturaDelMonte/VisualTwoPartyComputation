#pragma once
#ifndef __EXPRESSION_TESTER_H__
#define __EXPRESSION_TESTER_H__

#include <boost/variant/static_visitor.hpp>
#include "BooleanExpressions.h"

struct SExpressionSolver : public boost::static_visitor<void>
{
	CAssocVector<u32, bool>&		m_input;
	bool&							m_result;
public:

	SExpressionSolver(CAssocVector<u32, bool>& input, bool& result) :
		m_input(input), m_result(result)
	{	
	}

	void operator()(const Variable& v) const 
	{ 
		u16 op = atoi(*v);
		m_result = m_input[op];
	}

    void operator()(const SBinaryOperation<SAndOperation>& b) const 
	{ 
		bool left, right;
		boost::apply_visitor(SExpressionSolver(m_input, left), b.oper1);
		boost::apply_visitor(SExpressionSolver(m_input, right), b.oper2);
		m_result = left && right;
	}

    void operator()(const SBinaryOperation<SOrOperation>& b) const 
	{ 
		bool left, right;
		boost::apply_visitor(SExpressionSolver(m_input, right), b.oper2);
		boost::apply_visitor(SExpressionSolver(m_input, left), b.oper1);
		m_result = left || right;
	}

	void operator()(const SUnaryOperation<SNotOperation>& u, bool is_negate = false) const
    {
		bool res;
        boost::apply_visitor(SExpressionSolver(m_input, res), u.oper1);
		m_result = !res;
    }

};


#endif