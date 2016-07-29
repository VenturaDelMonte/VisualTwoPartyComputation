/*
	file name: Circuit_Recurse.cpp
	author: Ventura Del Monte & Anna De Angelis
	created on: 1/07/13
*/
#include "stdafx.h"
#include "Gate.h"
#include "Circuit.h"
#include "VisualCrypt.h"
#include "MVCSEncrypter.h"
#include "color_list.h"
#include "VCSEncrypter.h"
#include "MVCSDecrypter.h"
#include "RasterImage.h"
#include "VCSDecrypter.h"

extern void SerializeShare(LPCSTR name, CCircuit::CShare* sh, bool destroy_after = false);

void SerializeShare(LPCSTR name, u32 idx, u32 schema, u32 mode, CCircuit::CShare* sh)
{
	char str[128];
	sprintf(str, "%s_%d_%d_%d", name, idx, schema, mode);
	SerializeShare(str, sh);
}
#if 0
static const u16 LEFT_BRANCH		= 0;
static const u16 RIGHT_BRANCH		= 1;
class CSharesBuilderHelper
{
	friend class CSharesBuilderThread;
public:
	typedef CCircuit::CShare													CShare;
	typedef ISharesHolder<CShare>												CSharesHolder;
	typedef CSharesHolder::SharePair											CSharePair;
	typedef CVisualCrypter<CMVCSEncrypter<CShare>, CMVCSDecrypter<CShare>>		CMVCS;
	typedef CMVCS::CEncrypter::IMAGE_STORAGE									IMAGE_STORAGE;
	typedef CVector<CCircuit::position*>										NODES_STORAGE;


	CSharesBuilderHelper(CSharePair left, CSharePair right, CCircuit::position* dest, u16 mode, CCircuit::CGateTree& tree) : 
		m_left(left), m_right(right), m_dest(dest), m_mode(mode), m_tree(tree)
	{
		
	}

	CSharesBuilderHelper(CSharesHolder& holder, CCircuit::position* dest, u16 mode, CCircuit::CGateTree& tree) : 
		m_dest(dest), m_mode(mode), m_tree(tree), m_it(-1)
	{
		
		Setup(holder, tree.getIndex(m_dest));
		
	}

	virtual ~CSharesBuilderHelper()	
	{
	}

	CSharesHolder* Recurse()
	{
		CVector<CShare*> bits;
		const bool is_negate = m_dest->getElement()->IsNegate();
		GenerateGatesBits(bits, m_dest, is_negate);
#ifdef RANDOM_BIT

		float prob = CRandom(u32(GetQueryPerformaceCounter() & u32(-1))).RandF();
//		Log("Bit di permutazione per %d %f %d", m_tree.getIndex(m_dest), prob, prob > 0.5);
		dynamic_cast<CSharesHolder*>(m_dest->getElement())->SetInverted(prob > 0.5); // set b random
#endif

		// errato: considerare quando b=1 bisogna invertire, le shares.
		switch (m_dest->getElement()->GetType())
		{
			case IGate::eOrGate:
			{
				if (!is_negate)
				{
					Recurse(bits, m_left.second, m_right.second, 0);
					return Recurse(bits, m_right.second, m_right.second, 1);
				}
				else
				{
					Recurse(bits, m_right.second, m_left.second, 0);
					return Recurse(bits, m_left.second, m_left.second, 1);
				}
			}
			case IGate::eAndGate:
			{
				if (!is_negate)
				{
					Recurse(bits, m_left.second, m_left.second, 0);
					return Recurse(bits, m_left.second, m_right.second, 1);
				}
				else
				{
					Recurse(bits, m_right.second, m_right.second, 0);
					return Recurse(bits, m_right.second, m_left.second, 1);
				}
			}
			default: ASSERT(0, "invalid port"); return 0;
		}
	}

	CSharesHolder* Recurse(CVector<CShare*>& bits, CShare* img0, CShare* img1, u16 schema)
	{
		CMVCS mvcs; 

		ASSERT(img0 && img1, "invalid images");

		CSharesHolder* holder = dynamic_cast<CSharesHolder*>(m_dest->getElement()); // ASSERT(holder, "wrong obj");
		IMAGE_STORAGE res;
		mvcs.Encrypter().AddImage(img0).AddImage(img1).Encrypt(res);

		const u32 idx = m_tree.getIndex(m_dest);
		
		for (u32 k = 0; k < res.size(); k++)
		{
			ASSERT(res[k], "invalid image");
			CShare* bit = 0;

			if (bits.size())
			{
				CShare* tmp = bits[schema ? k + 3 : k]; //[mode * 2 + k + 1];
				if (k == 0)
				{
					CShare* ext_bit = 0;
#ifndef RANDOM_BIT
					ext_bit = schema == LEFT_BRANCH ? CBitmapEx::BitFalse() : CBitmapEx::BitTrue();
#else
					if (!holder->IsInverted())
						ext_bit = schema == LEFT_BRANCH ? CShare::BitFalse() : CShare::BitTrue();
					else
						ext_bit = schema == LEFT_BRANCH ? CShare::BitTrue() : CShare::BitFalse();
#endif
					bit = CShare::Join(ext_bit, tmp);
					Delete(ext_bit);
				}
				else
				{
					bit = tmp;
				}
			}
			else if (k == 0)
			{
#ifndef RANDOM_BIT
				bit = schema == LEFT_BRANCH ? CBitmapEx::BitFalse() : CBitmapEx::BitTrue();
#else
				if (!holder->IsInverted())
					bit = schema == LEFT_BRANCH ? CShare::BitFalse() : CShare::BitTrue();
				else
					bit = schema == LEFT_BRANCH ? CShare::BitTrue() : CShare::BitFalse();
#endif
			}
					
			(*holder)(schema, k, CSharePair(bit, res[k]));
			
		}
		return holder;
	}
	// holder del padre, mode posizione dell'elemento su cui lavoriamo
	void Setup(CSharesHolder& holder, u16 mode) 
	{
		switch (mode % 2)
		{
			case RIGHT_BRANCH:
			{
				CShare* l = 0;
				CRasterImage* r = 0;
				CRasterImage *l1 = holder(LEFT_BRANCH, 1).second, *l2 = holder(RIGHT_BRANCH, 1).second; // 2A||2B
				CRasterImage *r1 = holder(LEFT_BRANCH, 2).second, *r2 = holder(RIGHT_BRANCH, 2).second; // 3A||3B
#ifndef RANDOM_BIT
				l = CRasterImage::Join(l1, l2);
				r = CRasterImage::Join(r1, r2);
#else
#if 0
				if (holder.IsInverted())
				{
					l = CRasterImage::Join(l2, l1);
					r = CRasterImage::Join(r2, r1);
				}
				else
				{
					l = CRasterImage::Join(l1, l2);
					r = CRasterImage::Join(r1, r2);
				}
#else
				l = CRasterImage::Join(l1, l2);
				r = CRasterImage::Join(r1, r2);
#endif
#endif
				m_left = CSharePair(NULL, l);
			
				m_right = CSharePair(NULL, r);

				break;
			}
			case LEFT_BRANCH:
			{
				m_left.first = m_right.first = NULL;
				m_left.second = holder(LEFT_BRANCH, 0).second;		// 1A
				m_right.second = holder(RIGHT_BRANCH, 0).second;	// 1B
				
				break;
			}
			
		}
	}

	void GenerateGatesBits(CVector<CShare*>& ret, CCircuit::position* pos, const bool is_negate)
	{
		u32 idx = m_tree.getIndex(pos);
		IGate* g = pos->getElement();
		int layer = m_tree.depth(pos);
		int parent_layer = layer - 1;
		bool is_left = idx % 2 == 0;
		if (!layer)
			return;
		if (is_left && m_mode == LEFT_BRANCH)
			parent_layer++;
		else if (!is_left && m_mode != LEFT_BRANCH && parent_layer > 0)
			parent_layer--;
		switch (g->GetType())
		{
			case IGate::eAndGate:
			{
				if (parent_layer)
				{
					if (!is_negate)
					{
						for (int i = 0; i < 5; i++)
							ret.push_back(CShare::Repeat(CShare::BitFalse(), parent_layer));
						ret.push_back(CShare::Repeat(CShare::BitFalse(true), parent_layer));
					}
					else
					{
						ret.push_back(CShare::Repeat(CShare::BitFalse(), parent_layer));
						ret.push_back(CShare::Repeat(CShare::BitFalse(true), parent_layer));
						ret.push_back(CShare::Repeat(CShare::BitFalse(true), parent_layer));

						ret.push_back(CShare::Repeat(CShare::BitFalse(), parent_layer));
						ret.push_back(CShare::Repeat(CShare::BitFalse(true), parent_layer));
						ret.push_back(CShare::Repeat(CShare::BitFalse(), parent_layer));

					}
				}
				break;
			}
			case IGate::eOrGate:
			{
				// mode ? k + 3 : k
				if (parent_layer)
				{
					if (!is_negate)
					{
						for (int i = 0; i < 2; i++)
							ret.push_back(CShare::Repeat(CShare::BitFalse(), parent_layer));
						ret.push_back(CShare::Repeat(CShare::BitFalse(true), parent_layer));
						ret.push_back(CShare::Repeat(CShare::BitFalse(), parent_layer));
						for (int i = 0; i < 2; i++)
							ret.push_back(CShare::Repeat(CShare::BitFalse(true), parent_layer));
					}
					else
					{
						ret.push_back(CShare::Repeat(CShare::BitFalse(), parent_layer));
						ret.push_back(CShare::Repeat(CShare::BitFalse(true), parent_layer));
						ret.push_back(CShare::Repeat(CShare::BitFalse(), parent_layer));

						ret.push_back(CShare::Repeat(CShare::BitFalse(), parent_layer));
						ret.push_back(CShare::Repeat(CShare::BitFalse(), parent_layer));
						ret.push_back(CShare::Repeat(CShare::BitFalse(), parent_layer));
					}
				}	
				break;
			}
			default: break;
		}
	}

	

	void Init(CCircuit* circuit)
	{		
		if (m_it < 0)
		{
			circuit->GetChildren(m_nodes, m_dest);
			m_it = 0;
		}
	}

	bool CanProcess()
	{
		if (m_it < int(m_nodes.size()))
		{
			m_dest = m_nodes[m_it];
			return true;
		}
		return false;
		
	}

	s32 NextStep()
	{
		//m_dest = NULL;
		m_it++;
		return m_it;
	}

	u32 count() const
	{
		return m_nodes.size();
	}

protected:
	CSharePair							m_left;
	CSharePair							m_right;
	CCircuit::position*					m_dest;
	CCircuit::CGateTree&				m_tree;
	u16									m_mode;
	NODES_STORAGE						m_nodes;
	int									m_it;
};

class CSharesBuilderThread : public CThread
{
public:

	typedef CThread								base;

	typedef CSharesBuilderHelper::CSharesHolder	CSharesHolder;

	CSharesBuilderThread(volatile CThreadPool* pool, CSharesBuilderHelper *left, CCircuit* owner) : base(pool), m_owner(owner), m_left(left)
	{
	}

	virtual ~CSharesBuilderThread()	
	{
	}

	virtual void Run()
	{
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
		m_left->Init(m_owner);
		u32 count = m_left->count(), i = 0;
		while (m_left->CanProcess())
		{			
			CCircuit::position* pos = m_left->m_dest;
			CCircuit::position* ppos = m_left->m_tree.parent(pos);
			Sleep(50);
			if (!m_left->m_tree.isRoot(ppos))
				m_left->Setup(*(dynamic_cast<CSharesHolder*>(ppos->getElement())), m_left->m_tree.getIndex(pos));
			CSharesHolder* holder = m_left->Recurse();
			m_owner->OnNodeBuilded(pos);
			i = m_left->NextStep();
			m_progress = float(i / count);
		}
	}
private:
	CSharesBuilderHelper* m_left;
	CCircuit* m_owner;
};

#else
static const BOOL LEFT_BRANCH		= FALSE;
static const BOOL RIGHT_BRANCH		= TRUE;
class ABSTRACT CSharesBuilderHelper
{
public:
	typedef CCircuit::CShare													CShare;
	typedef CCircuit::position													position;
	typedef ISharesHolder<CShare>												CSharesHolder;
	typedef CSharesHolder::SharePair											CSharePair;
	typedef CVisualCrypter<CMVCSEncrypter<CShare>, CMVCSDecrypter<CShare>>		CMVCS;
	typedef CVisualCrypter<CVCSEncrypter<CShare>, CVCSDecrypter<CShare>>		CVCS;
	typedef CMVCS::CEncrypter::IMAGE_STORAGE									IMAGE_STORAGE;

	CSharesBuilderHelper(CCircuit::CGateTree& tree, BOOL branch) :
			m_tree(tree), m_branch(branch)
	{

	}

	virtual ~CSharesBuilderHelper()
	{
	}

	void Setup(CSharesHolder& holder, u16 mode, CSharePair& left, CSharePair& right) 
	{
		switch (mode)
		{
			case RIGHT_BRANCH:
			{
				CShare *l, *r, *bl, *br;
				l = r = bl = br = 0;

				CShare *l1 = holder(LEFT_BRANCH, 1).second, *l2 = holder(RIGHT_BRANCH, 1).second; // 2A||2B
				CShare *r1 = holder(LEFT_BRANCH, 2).second, *r2 = holder(RIGHT_BRANCH, 2).second; // 3A||3B

				CShare *bl1 = holder(LEFT_BRANCH, 1).first, *bl2 = holder(RIGHT_BRANCH, 1).first; // 2A||2B
				CShare *br1 = holder(LEFT_BRANCH, 2).first, *br2 = holder(RIGHT_BRANCH, 2).first; // 3A||3B
			
				l = CShare::Join(l1, l2);
				r = CShare::Join(r1, r2);
			
				if (bl1 && bl2)
					bl = CShare::Join(bl1, bl2);
				if (br1 && br2)
					br = CShare::Join(br1, br2);

				left = CSharePair(bl, l);
			
				right = CSharePair(br, r);

				break;
			}
			case LEFT_BRANCH:
			{
				left.first = holder(LEFT_BRANCH, 0).first;
				right.first = holder(RIGHT_BRANCH, 0).first;
				left.second = holder(LEFT_BRANCH, 0).second;		// 1A
				right.second = holder(RIGHT_BRANCH, 0).second;	// 1B
				
				break;
			}
			
		}
	}

	bool GetPermutationBit(u32 idx)
	{
		float prob = CRandom(u32(GetQueryPerformaceCounter() & u32(-1))).RandF();
		return prob >= 0.5f;
	}

	void Recurse(position* pos, const BOOL schema)
	{
		IGate* gate = pos->getElement();
		if (gate->GetType() == IGate::eInputGate)
			return;
		position* parent = m_tree.parent(pos);
		bool is_left = true;
		if (m_tree.hasRight(parent) && m_tree.right(parent) == pos)
			is_left = false;
		CSharePair left, right;
		CSharesHolder* parent_holder = dynamic_cast<CSharesHolder*>(parent->getElement());
		Setup(*parent_holder, is_left ? LEFT_BRANCH : RIGHT_BRANCH, left, right);
		Recurse(schema, pos, left, right, is_left);
		OnNodeBuilded(pos);
		if (m_tree.hasLeft(pos))
			Recurse(m_tree.left(pos), schema);
		if (m_tree.hasRight(pos))
			Recurse(m_tree.right(pos), schema); 
	}

	void Recurse(const BOOL schema, position* pos, CSharePair& left, CSharePair& right, const bool is_left)
	{
		IGate* gate = pos->getElement();
		bool is_negate = gate->IsNegate();
		CSharesHolder* pHolder = dynamic_cast<CSharesHolder*>(gate);
		pHolder->SetInverted(GetPermutationBit(pos->getIndex())); // set b random
		switch (gate->GetType())
		{
			case IGate::eOrGate:
			{
				if (!is_negate)
				{
					Recurse(LEFT_BRANCH, *pHolder, left, right, is_left);
					Recurse(RIGHT_BRANCH, *pHolder, right, right, is_left);
				}
				else
				{
					Recurse(LEFT_BRANCH, *pHolder, right, left, is_left);
					Recurse(RIGHT_BRANCH, *pHolder, left, left, is_left);
				}
				break;
			}
			case IGate::eAndGate:
			{
				CSharesHolder* pHolder = dynamic_cast<CSharesHolder*>(gate);
				if (!is_negate)
				{
					Recurse(LEFT_BRANCH, *pHolder, left, left, is_left);
					Recurse(RIGHT_BRANCH, *pHolder, left, right, is_left);
				}
				else
				{
					Recurse(LEFT_BRANCH, *pHolder, right, right, is_left);
					Recurse(RIGHT_BRANCH, *pHolder, right, left, is_left);
				}
				break;
			}
			default:
			{
				FATAL("unknown gate");
				break;
			}
		}
	}

	void GenerateBits(const BOOL schema, IMAGE_STORAGE& bits, CShare* left, CShare* right, const bool is_left)
	{
		SColor left_pixel, right_pixel;
		bits.push_back(NULL);
		bits.push_back(NULL);
		bits.push_back(NULL);
		if (!(left && right))
			return;
		left_pixel = left->Get(1, 0);
		right_pixel = right->Get(1, 0);
		if (is_left)
		{
			if (left_pixel == WHITEA && right_pixel == WHITEA)
			{
				bits[0] = CShare::BitFalse();
				bits[1] = CShare::BitFalse();
				bits[2] = CShare::BitFalse();
			}
			else if (left_pixel == WHITEA && right_pixel == BLACK)
			{
				bits[0] = CShare::BitFalse();
				bits[1] = CShare::BitFalse();
				bits[2] = CShare::BitFalse(true);
			}
			else if (left_pixel == BLACK && right_pixel == WHITEA)
			{
				bits[0] = CShare::BitFalse();
				bits[1] = CShare::BitFalse(true);
				bits[2] = CShare::BitFalse();
			}
			else if (left_pixel == BLACK && right_pixel == BLACK)
			{
				bits[0] = CShare::BitFalse();
				bits[1] = CShare::BitFalse(true);
				bits[2] = CShare::BitFalse(true);
			}
					
			CShare *tmp = left->SubImage(2, 0, left->getWidth() - 2, 1);
			for (u16 i = 0; i < 3; i++)
				bits[i] = CShare::Join(bits[i], tmp);
			Delete(tmp);
		}
		else
		{
			bits[0] = left;
			bits[1] = left;
			bits[2] = (left == right) ? left : right;
			
		}

		
		
	}

	void Recurse(const BOOL schema, CSharesHolder& holder, CSharePair& left, CSharePair& right, const bool is_left)
	{
		CMVCS mvcs;
		IMAGE_STORAGE shares, bits;
		const bool is_inverted = holder.IsInverted();

		mvcs.Encrypter().AddImage(left.second).AddImage(right.second).Encrypt(shares);
		GenerateBits(schema, bits, left.first, right.first, is_left);
		for (u32 k = 0; k < shares.size(); k++)
		{
//			ASSERT(shares[k], "invalid image");
			CShare* tmp = bits[k];
			CShare* bit = 0;
			if (k == 0)
			{
				if (!is_inverted)
					bit = schema == LEFT_BRANCH ? CShare::BitFalse() : CShare::BitTrue();
				else
					bit = schema == LEFT_BRANCH ? CShare::BitTrue() : CShare::BitFalse();
				if (tmp)
					bit = CShare::Join(bit, tmp);
			}
			else
			{
				bit = tmp;
			}
			
			if (k == 0)
			{
				holder(schema, k, CSharePair(bit, shares[k]));
			}
			else
			{
				const BOOL dest = (is_inverted) ? !schema : schema;
				holder(dest, k, CSharePair(bit, shares[k]));
			}
			
		}
	}

	virtual void OnNodeBuilded(position* pos)
	{

	}

protected:
	CCircuit::CGateTree&				m_tree;
	BOOL								m_branch;

};

class CSharesBuilderThread : public CSharesBuilderHelper, public CThread
{
	typedef CThread								base0;
	typedef CSharesBuilderHelper				base1;
public:
	CSharesBuilderThread(volatile CThreadPool* pool, CCircuit* circuit, position* root, BOOL branch) :
		base0(pool), base1(circuit->GetTree(), branch), m_circuit(circuit), m_root(root)
	{
	}

	virtual ~CSharesBuilderThread()	
	{
	}

	virtual void Run()
	{
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
		Recurse(m_root, m_branch);
		
	}

	virtual void OnNodeBuilded(position* pos)
	{
		base1::OnNodeBuilded(pos);
		m_circuit->OnNodeBuilded(pos);
		Sleep(50);
	}

protected:
	position* m_root;
	CCircuit* m_circuit;
};

#endif

void CCircuit::BuildShares()
{
	typedef CSharesBuilderHelper::CSharePair	CSharePair;
	typedef ISharesHolder<CShare>				CSharesHolder;

	CThreadPool* pool = New<CThreadPool>();

	//Log("Inizio fase di costruzione shares!");

	position* root = m_tree.root();


	u32 levels = getLevels();
	u32 des_size = (u32) std::pow(2, levels);
	if (!m_share_size)
		m_share_size = des_size;
#if 0
	CShare* true_value = CShare::True(m_share_size);
	CShare* false_value = CShare::False(m_share_size);

	CSharesBuilderHelper initer(CSharePair(0, false_value), CSharePair(0, true_value), root, LEFT_BRANCH, m_tree);

	CSharesHolder* holder1 = initer.Recurse(); 

	OnNodeBuilded(root);

	if (m_tree.hasLeft(root) && m_tree.hasRight(root))
	{

		CSharesBuilderHelper h1(*holder1, m_tree.left(root), LEFT_BRANCH, m_tree);
		CSharesBuilderHelper h2(*holder1, m_tree.right(root), RIGHT_BRANCH, m_tree);
		//CSharesHolder& holder, CCircuit::position* dest, u16 mode, CCircuit::CGateTree& tree
		CSharesBuilderThread left(pool, &h1, this);
		CSharesBuilderThread right(pool, &h2, this);
	}

#else
	CShare* true_value = CShare::True(m_share_size);
	CShare* false_value = CShare::False(m_share_size);
	CSharePair left = CSharePair(0, false_value);
	CSharePair right = CSharePair(0, true_value);
	CSharesBuilderHelper initer(m_tree, 0);
	initer.Recurse(LEFT_BRANCH, root, left, right, true);
	OnNodeBuilded(root);
	if (m_tree.hasLeft(root) && m_tree.hasRight(root))
	{
		CSharesBuilderThread left(pool, this, m_tree.left(root), LEFT_BRANCH);
		CSharesBuilderThread right(pool, this, m_tree.right(root), RIGHT_BRANCH);
	}

#endif

	Draw(pool);
	pool->WaitWorkers();
	Delete(pool);

//	Log("Fine fase di costruzione shares!");

}

