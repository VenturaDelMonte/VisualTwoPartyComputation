/*
	file name: Circuit_Solver.cpp
	author: Ventura Del Monte & Anna De Angelis
	created on: 1/07/13
*/
#include "stdafx.h"
#include "Circuit.h"
#include "Gate.h"
#include "VisualCrypt.h"
#include "MVCSEncrypter.h"
#include "color_list.h"
#include "VCSEncrypter.h"
#include "MVCSDecrypter.h"
//.#include "BitmapEx.h"
#include "VCSDecrypter.h"

extern void SerializeShare(LPCSTR name, CCircuit::CShare* sh, bool destroy_after = false);
	
void SerializeShare(LPCSTR prefix, u32 depth_suffix, CCircuit::CShare* sh)
{
	char name[128];	
	if (!sh) return;
	sprintf(name, "%s_%d", prefix, depth_suffix);	
	SerializeShare(name, sh);
}

bool bitPermutation(CCircuit::CShare* bits)
{
	if (bits->Get(0, 0) == BLACK && bits->Get(1, 0) == WHITEA)
		return false;
	if (bits->Get(0, 0) == BLACK && bits->Get(1, 0) == BLACK)
		return true;
	return false;
}

CCircuit::CShare* getBits(CCircuit::CShare* bits)
{
	u32 x = bits->getWidth() - 2;
	u32 y = bits->getHeight();
	CCircuit::CShare* ret = New<CCircuit::CShare>(x, y);
	for(u32 i = 2; i < bits->getWidth(); i++)
	{
		ret->Set(i - 2, 0, bits->Get(i, 0));
	}
	return ret;
}

void CCircuit::SetInput(CAssocVector<u32, bool>& input)
{
	CAssocVector<u32, bool>::iterator it, last;
	// qui ci vuole il metodo getLeaves
/*
	u32 leftmost_child = (u32) std::pow(2, m_tree.depth(m_tree.root()));
	for (it = input.begin(), last = input.end(); it != last; ++it)
	{
		CInputGate<CBitmapEx>* g = dynamic_cast<CInputGate<CBitmapEx>*>(m_tree.access(leftmost_child)->getElement()); // ASSERT(g, "wrong gate");
		g->SetInput(it->second);
		leftmost_child++;
//		Log("gate x%d has input x%d=%d", g->GetPortNumber(), it->first, it->second);
	}
*/
	CVector<position*> leaves;
	m_tree.getLeaves(leaves, m_tree.root());
	
	for (CVector<position*>::iterator it = leaves.begin(); it != leaves.end(); it++)
	{
		CInputGate* g = dynamic_cast<CInputGate*>((*it)->getElement()); // ASSERT(g, "wrong gate");
		u32 port_num = g->GetPortNumber();
		g->SetInput(input[port_num]);
//		Log("x%d=%d", port_num, input[port_num]);
	}

}

#if 0

class CCircuitSolver : public CThread
{
public:

	typedef CThread																	base;
	typedef CCircuit::position														SPosition;
	typedef CCircuit::CGateTree														CTree;
	typedef CCircuit::CShare														CShare;
	typedef ISharesHolder<CBitmapEx>												CSharesHolder;
	typedef CSharesHolder::SharePair												CSharePair;
	typedef CVisualCrypter<CVCSEncrypter<CBitmapEx>, CVCSDecrypter<CBitmapEx>>		CVCS;

	CCircuitSolver(volatile CThreadPool* pool, CTree* tree, SPosition* p)
		: base(pool), m_pos(p), m_tree(tree)
	{
		
	}

	virtual ~CCircuitSolver()	
	{
	}

	static bool bitPermutation(CBitmapEx* bits)
	{
		if(bits->Get(0, 0).Get() == BLACK.Get() && bits->Get(1, 0).Get() == WHITE.Get())
			return false;
		if(bits->Get(0, 0).Get() == BLACK.Get() && bits->Get(1, 0).Get() == BLACK.Get())
			return true;
		return false;
	}

	static CBitmapEx* getBits(CBitmapEx* bits)
	{
		u32 x = bits->getWidth() - 2;
		u32 y = bits->getHeight();
		CBitmapEx* ret = New<CBitmapEx>(x, y);
		for(u32 i = 2; i < bits->getWidth(); i++)
		{
			ret->Set(i - 2, 0, bits->Get(i, 0));
		}
		return ret;
	}

	virtual void Run()
	{
		Solve(m_pos);		
	}

	static void Solve(ISharesHolder<CBitmapEx>& holder, bool x, bool y, u32 idx)
	{
		CSharePair tmp;
		bool perm = bitPermutation(holder(x ? 1 : 0, 0).first);
		//Log("perm=%d x=%d y=%d", perm, x, y);
		//SerializeShare("ipartial_bit", idx, holder(x ? 1 : 0, 0).first);
#ifndef RANDOM_BIT
		if (y)
		{
			if (perm)
			{
				tmp = holder(1, 2);
			}
			else
			{
				tmp = holder(0, 2);
			}
		}
		else
		{
			if (perm)
			{
				tmp = holder(1, 1);
			}
			else
			{
				tmp = holder(0, 1);
			}
		}
#else
		if (y)
		{
			if (perm)
			{
				tmp = (!holder.IsInverted()) ? holder(1, 2) : holder(0, 2);
			}
			else
			{
				tmp = (!holder.IsInverted()) ? holder(0, 2) : holder(1, 2);
			}
		}
		else
		{
			if (perm)
			{
				tmp = (!holder.IsInverted()) ? holder(1, 1) : holder(0, 1);
			}
			else
			{
				tmp = (!holder.IsInverted()) ? holder(0, 1) : holder(1, 1);
			}
		}
#endif
		Solve(holder, holder(x ? 1 : 0, 0), tmp, idx);
	}

	static void Solve(ISharesHolder<CBitmapEx>& holder, CSharePair left, CSharePair right, u32 idx)
	{
		CVCS mvcs, vcs;
		const bool perm = bitPermutation(left.first);
		CBitmapEx *partial_bit = 0;

		mvcs.Decrypter().AddShare(left.second);
#ifdef DEBUG
		SerializeShare("choosenL", idx, left.second);
#endif
		if (left.second->getWidth() < right.second->getWidth())
		{
			u32 x = right.second->getWidth() / 2;
			u32 y = right.second->getHeight();
			CBitmapEx *l = New<CBitmapEx>(x, y);
			CBitmapEx *r = New<CBitmapEx>(x, y);
			CBitmapEx::Split(right.second, l, r);
#ifdef DEBUG
			SerializeShare("split_l", idx, l);
			SerializeShare("split_r", idx, r);
#endif
			mvcs.Decrypter().AddShare(perm ? r : l);
			Delete(perm ? l : r);
#ifdef DEBUG
			SerializeShare("choosenR", idx, perm ? r : l);
#endif
		}
		else
		{
#ifdef DEBUG
			SerializeShare("choosenR", idx, right.second);
#endif
			mvcs.Decrypter().AddShare(right.second);
		}

		if (left.first->getWidth() > 2)
		{
			CBitmapEx *ext = getBits(left.first);
			vcs.Decrypter().AddShare(ext);
			vcs.Decrypter().AddShare(right.first);
			partial_bit = vcs.Decrypter().Decrypt();
		}

		CShare* partial_image = mvcs.Decrypter().Decrypt();
		SerializeShare("partial_image", idx, partial_image);
				
		holder.SetResult(CSharePair(partial_bit, partial_image));
		if (partial_bit)
			SerializeShare("partial_bit", idx, partial_bit);
	}

	static void SolveLeaves(CTree* tree, SPosition* pos)
	{
		CBitmapEx* bit = 0;
		SPosition* left = tree->left(pos);
		SPosition* right = tree->right(pos);
		CSharesHolder* holder = dynamic_cast<CSharesHolder*>(pos->getElement());
		bool x = dynamic_cast<CInputGate*>(left->getElement())->GetInput();
		bool y = dynamic_cast<CInputGate*>(right->getElement())->GetInput();
		Solve(*holder, x, y, tree->getIndex(pos));
	}

	void Solve(SPosition* pos)
	{	
		if (m_tree->isInternal(pos) && m_tree->hasLeft(m_tree->left(pos)))
		{
			SPosition* l = m_tree->left(pos);
			SPosition* r = m_tree->right(pos);
			CSharesHolder* holder = dynamic_cast<CSharesHolder*>(pos->getElement());
			CSharesHolder* holder_left = dynamic_cast<CSharesHolder*>(l->getElement());
			CSharesHolder* holder_right = dynamic_cast<CSharesHolder*>(r->getElement());
			Solve(l);
			Solve(r);
			Solve(*holder, holder_left->GetResult(), holder_right->GetResult(), m_tree->getIndex(pos));
		}
		else
		{
			SolveLeaves(m_tree, pos);
		}
	}
 


private:
	SPosition*	m_pos;
	CTree*		m_tree;

};

void CCircuit::Solve(CAssocVector<u32, bool>& input)
{
	typedef ISharesHolder<CBitmapEx>		CSharesHolder;
	typedef CSharesHolder::SharePair		CSharePair;

	//if (true) return;

	Log("Inizio fase di sovrapposizione shares!");

	position* root = m_tree.root();
	
	SetInput(input);
	if (m_tree.hasLeft(m_tree.left(root)))
	{
		CThreadPool pool;
		CCircuitSolver solver_left(&pool, &m_tree, m_tree.left(root)), solver_right(&pool, &m_tree, m_tree.right(root));
	
		solver_left.Start();
		solver_right.Start();

		pool.WaitWorkers();
		CSharesHolder* holder_left = dynamic_cast<CSharesHolder*>(m_tree.left(root)->getElement());
		CSharesHolder* holder_right = dynamic_cast<CSharesHolder*>(m_tree.right(root)->getElement());

		CCircuitSolver::Solve(*dynamic_cast<CSharesHolder*>(root->getElement()), holder_left->GetResult(), holder_right->GetResult(), 1);

		

	}
	else
	{
		CCircuitSolver::SolveLeaves(&m_tree, root);
	}

	Log("Fine fase di sovrapposizione shares!");

}

#else

class CCircuitSolver
{
public:
	typedef CCircuit::position														SPosition;
	typedef CCircuit::CGateTree														CTree;
	typedef CCircuit::CShare														CShare;
	typedef ISharesHolder<CShare>													CSharesHolder;
	typedef CSharesHolder::SharePair												CSharePair;
	typedef CVisualCrypter<CVCSEncrypter<CShare>, CVCSDecrypter<CShare>>			CVCS;
	typedef CVisualCrypter<CMVCSEncrypter<CShare>, CMVCSDecrypter<CShare>>			CMVCS;
	
	CCircuitSolver(CTree* tree, SPosition* p)
		: m_pos(p), m_tree(tree)
	{
		
	}

	virtual ~CCircuitSolver()	
	{

	}

	void MergeShare(CSharesHolder& holder, CSharePair& left, CSharePair& right, u32 idx)
	{
		CVCS mvcs, vcs;
		CShare* partial_bit = 0;
		ASSERT(left.second && right.second, "no image given");
#if 0
		SerializeShare("choosenL", idx, left.second);
		SerializeShare("choosenLb", idx, left.first);
		SerializeShare("choosenR", idx, right.second);
		SerializeShare("choosenRb", idx, right.first);
#endif
		CShare* partial_image = mvcs.Decrypter().AddShare(left.second).AddShare(right.second).Decrypt();
		if (left.first->getWidth() > 2)
		{
			CShare *ext = getBits(left.first);
			if (right.first)
			{
				vcs.Decrypter().AddShare(ext);
				vcs.Decrypter().AddShare(right.first);
				partial_bit = vcs.Decrypter().Decrypt();
			}
			else
			{
				partial_bit = CShare::Clone(ext);
			}
			Delete(ext);
		}
/*
		SerializeShare("partial_image", idx, partial_image);
				
		
		if (partial_bit)
			SerializeShare("partial_bit", idx, partial_bit);
*/

		holder.SetResult(CSharePair(partial_bit, partial_image));
	}

	void Solve()
	{
		Solve(m_pos);
	}

	void Solve(SPosition* pos)
	{
		SPosition *l, *r;
		if (m_tree->isInternal(pos))
		{
			Sleep(100);
			CSharesHolder& holder = *dynamic_cast<CSharesHolder*>(pos->getElement());
			
			l = m_tree->left(pos);
			r = m_tree->right(pos);
		
			Solve(l);
			Solve(r);
			
			SolveNode(pos, l, r);
			OnNodeSolved(pos);
		}
	}

	void SolveNode(SPosition* pos, SPosition* l, SPosition* r)
	{
		bool perm = false;
		CSharePair left, right;
		CSharesHolder& holder = *dynamic_cast<CSharesHolder*>(pos->getElement());
		bool unbalanced = false;
		if (m_tree->isInternal(l))
		{
			CSharesHolder* holder_left = dynamic_cast<CSharesHolder*>(l->getElement());
			left = holder_left->GetResult();
			perm = bitPermutation(left.first);
		}
		else
		{
			const bool x = dynamic_cast<CInputGate*>(l->getElement())->GetInput();
			left = holder(x ? 1 : 0, 0);
			perm = bitPermutation(left.first);
			unbalanced = true;
		}
		if (m_tree->isInternal(r))
		{
			CSharesHolder* holder_right = dynamic_cast<CSharesHolder*>(r->getElement());
			CSharePair tmp = holder_right->GetResult();
			if (left.second->getWidth() < tmp.second->getWidth())
			{
				u32 x = tmp.second->getWidth() / 2;
				u32 y = tmp.second->getHeight();
				CShare* left_share = New<CShare>(x, y);
				CShare* right_share = New<CShare>(x, y);
				CShare::Split(tmp.second, left_share, right_share);
				if (unbalanced)
				{
					//if (!holder.IsInverted())
					{
						right = CSharePair(tmp.first, perm ? right_share : left_share);
						Delete(perm ? left_share : right_share);
					}
					//else
					//{
				//		right = CSharePair(tmp.first, perm ? left_share : right_share);
				//		Delete(perm ? right_share : left_share);
				//	}
				}
				else
				{
					right = CSharePair(tmp.first, perm ? right_share : left_share);
					Delete(perm ? left_share : right_share);
				}
			}
			else
			{
				right = tmp;
			}
		}
		else
		{
			const bool y = dynamic_cast<CInputGate*>(r->getElement())->GetInput();
			bool is_inverted = holder.IsInverted();
			if (!unbalanced)
				is_inverted = false;
			if (y)
			{
				if (perm)
				{
					//right = (!is_inverted) ? holder(1, 2) : holder(0, 2);
					right = holder(1, 2);
				}
				else
				{
				//	right = (!is_inverted) ? holder(0, 2) : holder(1, 2);
					right = holder(0, 2);
				}
			}
			else
			{
				if (perm)
				{
					//right = (!is_inverted) ? holder(1, 1) : holder(0, 1);
					right = holder(1, 1);
				}
				else
				{
					//right = (!is_inverted) ? holder(0, 1) : holder(1, 1);
					right = holder(0, 1);
				}
			}
		}
		MergeShare(holder, left, right, pos->getIndex());
	}

	virtual void OnNodeSolved(SPosition* x)
	{
	}

protected:
	SPosition*	m_pos;
	CTree*		m_tree;
};

class CCircuitSolverThread : public CThread, public CCircuitSolver
{
	
public:

	typedef CThread											base0;
	typedef CCircuitSolver									base1;
	typedef fastdelegate::FastDelegate1<SPosition*>			OnCompleteCallback;
	

	CCircuitSolverThread(volatile CThreadPool* pool, OnCompleteCallback callback, CTree* tree, SPosition* p, u32 internal_nodes)
		: base0(pool), base1(tree, p), m_internal_nodes(internal_nodes), m_solved_nodes(0), m_callback(callback)
	{
		
	}

	virtual ~CCircuitSolverThread()	
	{

	}

	virtual void Run()
	{
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
		Solve(m_pos);		
	}

	virtual void OnNodeSolved(SPosition* x)
	{
		base1::OnNodeSolved(x);
		m_solved_nodes++;
		m_progress = float(m_solved_nodes) / float(m_internal_nodes);
		m_callback(x);
	}

private:
	u32 m_internal_nodes;
	u32 m_solved_nodes;
	OnCompleteCallback m_callback;

};

CCircuit::CShare* CCircuit::Solve(CAssocVector<u32, bool>& input)
{
	CShare* ret = 0;

	Log("Inizio fase di sovrapposizione shares!");

	position* root = m_tree.root();
	
	SetInput(input);
	u32 internal_nodes = m_tree.Size() - input.size();
	{
		CThreadPool pool;
		CCircuitSolverThread solver_left(&pool, fastdelegate::FastDelegate1<position*>(this, &CCircuit::OnNodeSolved), &m_tree, m_tree.left(root), internal_nodes / 2),
					solver_right(&pool, fastdelegate::FastDelegate1<position*>(this, &CCircuit::OnNodeSolved), &m_tree, m_tree.right(root), internal_nodes / 2);
	
		solver_left.Start();
		solver_right.Start();

		pool.WaitWorkers();
		
		CSharesHolder* holder = dynamic_cast<CSharesHolder*>(root->getElement());
		CCircuitSolver solver_root(&m_tree, root);
		solver_root.SolveNode(root, m_tree.left(root), m_tree.right(root));
		ret = CRasterImage::Clone(holder->GetResult().second);
		OnNodeSolved(root);
	}

	Log("Fine fase di sovrapposizione shares!");
	return ret;

}

#endif