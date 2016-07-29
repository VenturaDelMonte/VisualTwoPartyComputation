#include "stdafx.h"
#include "DescentParser.h"
#include "AvailableOperations.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include "BooleanExpressions.h"
#include "Circuit.h"

template 
<
	typename Iterator,
	typename Skipper = boost::spirit::qi::space_type
>
struct SParserImpl : boost::spirit::qi::grammar<Iterator, CExpression(), Skipper>
{
public:
    SParserImpl() : SParserImpl::base_type(m_expr)
    {
		using namespace boost::spirit::qi;

        m_expr  = m_or.alias();

        m_or  = ( m_and >> "+"  >> m_or )	[ _val = boost::phoenix::construct<SBinaryOperation<SOrOperation>> (_1, _2)] | m_and	[ _val = _1 ];
        m_and = ( m_not >> "*" >>  m_and)	[ _val = boost::phoenix::construct<SBinaryOperation<SAndOperation>>(_1, _2)] | m_not	[ _val = _1 ];
        m_not = ( "!" > m_simple		)	[ _val = boost::phoenix::construct<SUnaryOperation<SNotOperation>> (_1)    ] | m_simple	[ _val = _1 ];

        m_simple = (('(' > m_expr > ')') | m_var);
        m_var	 = lexeme[ (lit('x') > +digit) ];

        BOOST_SPIRIT_DEBUG_NODE(m_expr);
        BOOST_SPIRIT_DEBUG_NODE(m_or);
        BOOST_SPIRIT_DEBUG_NODE(m_and);
        BOOST_SPIRIT_DEBUG_NODE(m_not);
        BOOST_SPIRIT_DEBUG_NODE(m_simple);
        BOOST_SPIRIT_DEBUG_NODE(m_var);
    }

private:
    boost::spirit::qi::rule<Iterator, Variable(), Skipper> m_var;
    boost::spirit::qi::rule<Iterator, CExpression(), Skipper> m_not, m_and, m_or, m_simple, m_expr;
};

class CVariablesCounter : public boost::static_visitor<void>
{
private:
	u32& m_count;
	u64& m_variables_used;
	u16& m_max_variable;
public:
	CVariablesCounter(u32& count, u64& variables_used, u16& max_variable) 
		: m_count(count), m_variables_used(variables_used), m_max_variable(max_variable)
	{ 

	}

	void operator()(const Variable& v) const
	{ 
		u16 op = atoi(*v);
		ASSERT(op, "x0 is forbidden");
		u64 x = (1 << (op - 1));
		ASSERT(!(x & m_variables_used), *format("duplicate variable x%d", op));
		m_variables_used |= x;
		m_count++;
		m_max_variable = (op > m_max_variable) ? op : m_max_variable;
	}

    void operator()(const SBinaryOperation<SAndOperation>& b) const
	{ 
		boost::apply_visitor(CVariablesCounter(m_count, m_variables_used, m_max_variable), b.oper1);
		boost::apply_visitor(CVariablesCounter(m_count, m_variables_used, m_max_variable), b.oper2);
	}

    void operator()(const SBinaryOperation<SOrOperation>& b) const
	{ 
		boost::apply_visitor(CVariablesCounter(m_count, m_variables_used, m_max_variable), b.oper2);
		boost::apply_visitor(CVariablesCounter(m_count, m_variables_used, m_max_variable), b.oper1);
	}

	void operator()(const SUnaryOperation<SNotOperation>& u) const
    {
        boost::apply_visitor(CVariablesCounter(m_count, m_variables_used, m_max_variable), u.oper1);
    }

};



CDescentParser::CDescentParser(CCircuit& c) : 
	m_circuit(c), m_variable_count(0)
{

}

CDescentParser::~CDescentParser()
{

}


bool CDescentParser::Parse(string func)
{
	string::iterator f(std::begin(func)), l(std::end(func));
	SParserImpl<string::iterator> p;
	Log("Parsing %s", *func);
	try
	{
		CExpression result;
		bool ok = boost::spirit::qi::phrase_parse(f, l, p, boost::spirit::qi::space, result);

		ASSERT(ok, "invalid input");
		u64 mask = 0, test_mask = 0;
		u16 max_x = 0;
		boost::apply_visitor(CVariablesCounter(m_variable_count, mask, max_x), result);
		for (u16 i = 0; i < max_x; i++)
			test_mask |= u64(1i64 << u64(i));
		ASSERT(!(mask ^ test_mask), "missing variables!");
		boost::apply_visitor(m_circuit, result); 
	
	} 
	catch (const boost::spirit::qi::expectation_failure<string::iterator>& e)
	{
		FATAL(*format("failure at '%s'", *string(e.first, e.last)));
		return false;
	}
	catch (const std::exception & ex)
	{
		FATAL(*format("%s at '%s'", ex.what(), *string(f,l)));
		return false;
	}
	catch (...)
	{
		FATAL(*format("... at '%s'", *string(f,l)));
		return false;
	}
	if (f != l) 
	{
		FATAL(*format("unparsed: %s", *string(f,l)));
		return false;
	}
	return true;
    
}
