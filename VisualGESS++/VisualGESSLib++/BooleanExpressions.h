#pragma once
#ifndef __BOOLEAN_EXPRESSION_H__
#define __BOOLEAN_EXPRESSION_H__

#include <boost/variant/recursive_wrapper.hpp>
#include <boost/variant/variant.hpp>

struct SOrOperation  { };
struct SAndOperation { };
struct SNotOperation { };

typedef string Variable;
template <typename OperationType> struct SBinaryOperation;
template <typename OperationType> struct SUnaryOperation;

typedef boost::variant
<		
	Variable, 
    boost::recursive_wrapper<SUnaryOperation<SNotOperation>>, 
    boost::recursive_wrapper<SBinaryOperation<SAndOperation>>,
    boost::recursive_wrapper<SBinaryOperation<SOrOperation>>
> CExpression;

template 
<
	typename OperationType
> 
struct SBinaryOperation 
{ 
    explicit SBinaryOperation(const CExpression& l, const CExpression& r) : oper1(l), oper2(r) { }
    CExpression oper1, oper2; 
};

template 
<
	typename OperationType
> 
struct SUnaryOperation  
{ 
    explicit SUnaryOperation(const CExpression& o) : oper1(o) { }
    CExpression oper1; 
};

#endif