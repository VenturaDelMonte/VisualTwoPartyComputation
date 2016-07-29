#pragma once
#ifndef __TREE_BUILDER_H__
#define __TREE_BUILDER_H__

#include <boost/variant/static_visitor.hpp>
#include "LinkedTree.h"
#include "Gate.h"
#include "BooleanExpressions.h"

class IGate;

template <typename ImageType>
struct STreeBuilder : public boost::static_visitor<void>
{
	typedef CLinkedBinaryTree<IGate*>				CAbstractSyntaxTree;
	typedef CAbstractSyntaxTree::SBTreeNode			SBTreeNode;

private:
			CAbstractSyntaxTree*			m_ast;
	mutable SBTreeNode*						m_start_node;
			bool							m_negate_first;

public:

	STreeBuilder(CAbstractSyntaxTree* ast, SBTreeNode* start_node = 0, bool negate_first = false)
	{
		m_ast = ast;
		m_start_node = start_node;
		m_negate_first = negate_first;
	}

	void operator()(const Variable& v) const 
	{ 
		s16 op = atoi(*v);
		IGate* g = New<CInputGate>(op);
		g->SetNegate(m_negate_first);
		//Log("=> %d", op);
		ASTInsert(g);
	}

    void operator()(const SBinaryOperation<SAndOperation>& b) const 
	{ 
		Log("and matched");
		IGate* g = New<CAndGate<ImageType>>();
		g->SetNegate(m_negate_first);
		SBTreeNode* node = ASTInsert(g);
		Recurse(node, b.oper1, b.oper2); 
	}

    void operator()(const SBinaryOperation<SOrOperation>& b) const 
	{ 
		Log("or matched");
		IGate* g = New<COrGate<ImageType>>();
		g->SetNegate(m_negate_first);
		SBTreeNode* node = ASTInsert(g);
		Recurse(node, b.oper1, b.oper2); 
	}

	void operator()(const SUnaryOperation<SNotOperation>& u) const
    {
        Log("not matched");
	//	STreeNode* node = ASTInsert(NOT_OPERATION);
        boost::apply_visitor(STreeBuilder(m_ast, m_start_node, true), u.oper1);
    }

protected:

	void Recurse(SBTreeNode* node, const CExpression& l, const CExpression& r) const
    {
        boost::apply_visitor(STreeBuilder(m_ast, node), l);
        boost::apply_visitor(STreeBuilder(m_ast, node), r);
    }

	SBTreeNode* ASTInsert(IGate* g) const
	{
		SBTreeNode* node = (SBTreeNode*) m_ast->insert(m_start_node, g);
		g->SetPosition(node);
		return node;

	}
};


#endif