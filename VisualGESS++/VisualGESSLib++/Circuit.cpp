/*
	file name: Circuit.cpp
	author: Ventura Del Monte & Anna De Angelis
	created on: 11/04/2013
*/
#include "stdafx.h"
#include "AvailableOperations.h"
#include "Circuit.h"
#include "CircuitIO.h"
#include "Gate.h"

#define DEFAULT_MARGIN 0

extern string g_output_folder;

#define DEFAULT_EXTENSION "png"

struct SCountLayerBfs
{
private:
	u32&												m_max_layer;	
public:
	explicit SCountLayerBfs(u32& max_layer) 
		: m_max_layer(max_layer)
	{
	}

	void operator()(u32 idx, CCircuit::position* pos) const
	{
		m_max_layer = max(m_max_layer, idx);
	//	Log("%d %p", idx, pos);
	}

};

void CCircuit::Init() 	
{ 
	
}	

CCircuit::~CCircuit()
{
//	Log("%s", m_tree.toString(m_tree.root()).c_str());
	struct SSharesDestroyer
	{
		void operator() (CCircuit::position* pos) const
		{
			pos->getElement()->Destroy();
		}
	} destroyer;
	if(m_tree.Size())
		m_tree.postOrderVisit(m_tree.root(), destroyer);
}
/*
void CCircuit::BuildCallback(s16 op)
{
	IPureGate*				g	= 0;
	position*				p	= 0;
	switch (op)
	{
		case OR_OPERATION:
		{
			g					= New<COrGate<CShare>>						();
			p					= m_tree.add								(g);

			Log																("or gate added [0x%x]", g);
			break;
		}
		case AND_OPERATION:
		{
			g					= New<CAndGate<CShare>>						();
			p					= m_tree.add								(g);
			Log																("and gate added [0x%x]", g);
			break;
		}
		case NOT_OPERATION:
		{
			Log("not should be applied");
			break;
		}
		default:
		{
			g					= New<CInputGate<CShare>>					(op);
			p					= m_tree.add								(g);		
			Log																("input x%d gate added [0x%x]", op, g);
			break;
		}
	}

//	SetPositions															(g, p);
	if (g)
		g->SetPosition															(p);
	
}
*/
/*
CCircuit::return_type CCircuit::Execute()
{
	IPureGate* root = m_tree.root()->getElement();
	Log("depth tree: %d\n", m_tree.depth());
//	return root->execute();
}
*/
#if 0
void CCircuit::SetPositions(IPureGate* g, position *p)
{

	if (!m_tree.isRoot(p))
	{
		position*	parent	= m_tree.parent					(p);
		IPureGate*	q		= parent->getElement			();
		if (!m_tree.hasLeft(parent) && !m_tree.hasRight(parent))
		{
			q->SetRight										(g);
			Log("[0x%x] has right [0x%x]", q, g);
		}
		else if (!m_tree.hasLeft(parent) && m_tree.hasRight(parent))
		{
			q->SetRight										(g);
			Log("[0x%x] has right [0x%x]", q, g);
		}
		else if (!m_tree.hasRight(parent) && m_tree.hasLeft(parent))
		{
			q->SetLeft										(g);
			Log("[0x%x] has left [0x%x]", q, g);
		}
		else
		{
//			Error("error, no valid position");
		}
		g->SetOut											(q);
		Log("[0x%x] has out [0x%x]", g, q);
		
	}
	else
	{
		Log("[0x%x] is the root", g);
		COutputGate<CShare>* out = 
					New<COutputGate<CShare>>				();
		g->SetOut											(dynamic_cast<IPureGate*>(out));
	}

}
#endif

/**
	Get children of position g
*/
void CCircuit::GetChildren(CVector<CCircuit::position*>& vec, CCircuit::position* g)
{
	if (m_tree.isInternal(g))
	{
		CVector<CCircuit::position*> l, r;
		vec.push_back(g);
		GetChildren(l, m_tree.left(g));
		GetChildren(r, m_tree.right(g));
		if (l.size()) 
		{
			for (CVector<CCircuit::position*>::iterator it = l.begin();
				it != l.end();
				++it)
			{
				vec.push_back(*it);
			}
		}
		if (r.size())
		{
			for (CVector<CCircuit::position*>::iterator it = r.begin();
				it != r.end();
				++it)
			{
				vec.push_back(*it);
			}
		}
	}
}

void SerializeShare(LPCSTR name, CCircuit::CShare* sh, bool destroy_after = false)
{
	char path[128];
	sprintf(path, "%s\\%s.%s", *g_output_folder, name, DEFAULT_EXTENSION);
	CWriter* w = FS.w_open(path);				
	sh->Save(w);								
	Delete(w);	
	if (destroy_after)
		Delete(sh);
	Log("stored %s", path);
}

CCircuit::CShare* CCircuit::ConcatenateShare(LPCSTR name, CSharePair& p)
{
	return ConcatenateShare(name, p, m_expansion, m_share_size);
}

CCircuit::CShare* CCircuit::ConcatenateShare(LPCSTR name, CSharePair& p1, CSharePair& p2)
{
	return ConcatenateShare(name, p1, p2, m_expansion, m_share_size);
}

#if 0

CCircuit::CShare* CCircuit::ConcatenateShare(LPCSTR name, CSharePair& p, u32 expansion, u32 shares_size)
{
	u32 o_width = p.second->getWidth();
	u32 o_height =  p.second->getHeight();
	CShare* ret = 0, *label = 0;
	CShare* ebits = (p.first) ? (expansion > 1) ? p.first->ExpandGrid(expansion) : p.first->Expand(1) : 0;
	CShare* eshare = (expansion > 1) ? p.second->ExpandGrid(expansion) : p.second->Expand(1);
	if (ebits)
	{
		u32 txy = 0;
		u32 subshares_count = o_width / shares_size;
		u32 bits = p.first->getWidth() / 2;
		u32 e_height = eshare->getHeight();
		u32 eb_width = ebits->getWidth();
		u32 new_width = 2 + eshare->getWidth() + ebits->getWidth();
		u32 new_height = e_height + ebits->getHeight() + (expansion > 1 ? 50 : 0);
		if (name) txy = 50;
		new_height += txy;
		txy /= 2;
		ret = New<CShare>(new_width, new_height);
		ret->Fill(WHITEA);
		ret->insertImage(eshare, 1 + eb_width, (expansion > 1 ? 25 + txy : 0));
		ret->insertImage(ebits, 1, (expansion > 1 ? 25 + txy : 0));
		if (expansion <= 1)
			return ret;
		if (name)
		{
			label = CRasterImage::CreateText(name + 6, 1, 20, 100, 18, 28);
			ret->insertImage(label, 20, 5);
		}		
		for (u32 y = txy + 5; y < e_height + 47 + txy; y++)
		{
			ret->Set(0, y, BLACK);
			ret->Set(new_width - 1, y, BLACK);
			ret->Set(1, y, BLACK);
			ret->Set(new_width - 2, y, BLACK);
			ret->Set(eb_width, y, BLACK);
			ret->Set(1 + eb_width, y, BLACK);
			for (u32 i = 1; i < subshares_count; i++)
			{
				ret->Set(1 + eb_width + i * (eshare->getWidth() / subshares_count), y, BLACK);
			}
			for (u32 i = 1; i < bits; i++)
			{
				if (y >= txy + 50) break;
				else if (y < txy + 10) continue;
				ret->Set(1 + i * (eb_width / bits), y, BLACK);
			}	
		}
		for (u32 x = 0; x < new_width; x++)
		{
			ret->Set(x, txy + 25, BLACK);
			if (x < eb_width) continue;
			ret->Set(x, txy + 24 + e_height, BLACK);
		}
	}
	else
	{
		u32 subshares_count = o_width / shares_size;
		u32 e_height = eshare->getHeight();
		u32 new_width = eshare->getWidth() + 2;
		u32 new_height = eshare->getHeight() + (expansion > 1 ? 50 : 0);
		ret = New<CShare>(new_width, new_height);
		ret->Fill(WHITEA);
		ret->insertImage(eshare, 1, (expansion > 1 ? 25 : 0));
		if (name && expansion > 1)
		{
			label = CRasterImage::CreateText(name + 6, 1, 20, 100, 18, 28, WHITEA);
			ret->insertImage(label, 20, 5);
		}
		if (expansion <= 1)
			return ret;
		for (u32 y = 5; y < e_height + 47; y++)
		{
			ret->Set(0, y, BLACK);
			ret->Set(new_width - 1, y, BLACK);
			ret->Set(1, y, BLACK);
			ret->Set(new_width - 2, y, BLACK);
			for (u32 i = 1; i < subshares_count; i++)
			{
				ret->Set(1 + i * (eshare->getWidth() / subshares_count), y, BLACK);
			}
		}
	}
	Delete(label);
	Delete(eshare);
	Delete(ebits);
	return ret;
}

CCircuit::CShare* CCircuit::ConcatenateShare(LPCSTR name, CSharePair& p1, CSharePair& p2, u32 expansion, u32 shares_size)
{
	CShare* sh1 = ConcatenateShare(0, p1, expansion, shares_size);
	CShare* sh2 = ConcatenateShare(0, p2, expansion, shares_size);
	u32 w = sh1->getWidth() + sh2->getWidth();
	u32 h = sh1->getHeight() + (expansion > 1 ? 50 : 0);
	CShare* ret = New<CShare>(w, h);
	CRasterImage* label = 0;
	ret->Fill(WHITEA);
	
	ret->insertImage(sh1, 0, (expansion > 1 ? 40 : 0));
	ret->insertImage(sh2, sh1->getWidth() - 2, (expansion > 1 ? 40 : 0));

	
	if (name && expansion > 1)
	{
		label = CRasterImage::CreateText(name + 6, 1, 20, 100, 18, 28, WHITEA);
		ret->insertImage(label, 20, 5);
	}

	Delete(label);
	Delete(sh1);
	Delete(sh2);

	return ret;
}

#else

CCircuit::CShare* AddLabel(LPCSTR name, CCircuit::CShare* img)
{
	if (!name)
		return img;
	u32 w = img->getWidth();
	u32 h =  img->getHeight() + 50;
	CCircuit::CShare* ret = New<CCircuit::CShare>(w, h);
	ret->insertImage(img, 0, 25);
	CRasterImage* label = CRasterImage::CreateText(name + 6, 1, 20, 100, 18, 28, WHITEA);
	ret->insertImage(label, w / 2, 5);
	Delete(img);
	return ret;
}

CCircuit::CShare* CCircuit::ConcatenateShare(LPCSTR name, CSharePair& p, u32 expansion, u32 shares_size)
{
	u32 o_width = p.second->getWidth();
	u32 o_height =  p.second->getHeight();
	CShare* ret = 0, *label = 0;
	CShare* ebits = (p.first) ? (expansion > 1) ? p.first->ExpandGrid(expansion) : p.first->Expand(1) : 0;
	CShare* eshare = p.second->ExpandGrid(expansion);
	if (ebits)
	{
		u32 subshares_count = o_width / shares_size;
		u32 bits = p.first->getWidth() / 2;
		u32 e_height = eshare->getHeight();
		u32 eb_width = ebits->getWidth();
		u32 new_width = 2 + eshare->getWidth() + ebits->getWidth();
		u32 new_height = e_height + 50;
		ret = New<CShare>(new_width, new_height);
		ret->Fill(WHITEA);
		ret->insertImage(eshare, 1 + eb_width, (expansion > 1 ? 25 : 0));
		ret->insertImage(ebits, 1, (expansion > 1 ? 25 : 0));		
		for (u32 y = 5; y < e_height + 47; y++)
		{
			ret->Set(0, y, BLACK);
			ret->Set(new_width - 1, y, BLACK);
			ret->Set(1, y, BLACK);
			ret->Set(new_width - 2, y, BLACK);
			ret->Set(eb_width, y, BLACK);
			ret->Set(1 + eb_width, y, BLACK);
			for (u32 i = 1; i < subshares_count; i++)
			{
				ret->Set(1 + eb_width + i * (eshare->getWidth() / subshares_count), y, BLACK);
			}
			for (u32 i = 1; i < bits; i++)
			{
				if (y >= 50) break;
				else if (y < 10) continue;
				ret->Set(1 + i * (eb_width / bits), y, BLACK);
			}	
		}
		for (u32 x = 0; x < new_width; x++)
		{
			ret->Set(x, 25, BLACK);
			if (x < eb_width) continue;
			ret->Set(x, 24 + e_height, BLACK);
		}
	}
	else
	{
		u32 subshares_count = o_width / shares_size;
		u32 e_height = eshare->getHeight();
		u32 new_width = eshare->getWidth() + 2;
		u32 new_height = eshare->getHeight() + 50;
		ret = New<CShare>(new_width, new_height);
		ret->Fill(WHITEA);
		ret->insertImage(eshare, 1, (expansion > 1 ? 25 : 0));

		for (u32 y = 5; y < e_height + 47; y++)
		{
			ret->Set(0, y, BLACK);
			ret->Set(new_width - 1, y, BLACK);
			ret->Set(1, y, BLACK);
			ret->Set(new_width - 2, y, BLACK);
			for (u32 i = 1; i < subshares_count; i++)
			{
				ret->Set(1 + i * (eshare->getWidth() / subshares_count), y, BLACK);
			}
		}
	}
	Delete(label);
	Delete(eshare);
	Delete(ebits);
	return AddLabel(name, ret);
}

/*

if (name)
		{
			label = CRasterImage::CreateText(name + 6, 1, 20, 100, 18, 28);
			ret->insertImage(label, 20, 5);
		}

	if (name && expansion > 1)
		{
			label = CRasterImage::CreateText(name + 6, 1, 20, 100, 18, 28, WHITEA);
			ret->insertImage(label, 20, 5);
		}
*/

CCircuit::CShare* CCircuit::ConcatenateShare(LPCSTR name, CSharePair& p1, CSharePair& p2, u32 expansion, u32 shares_size)
{
	CShare* sh1 = ConcatenateShare(0, p1, expansion, shares_size);
	CShare* sh2 = ConcatenateShare(0, p2, expansion, shares_size);
	u32 w = sh1->getWidth() + sh2->getWidth();
	u32 h = sh1->getHeight();
	CShare* ret = New<CShare>(w, h);
	CRasterImage* label = 0;
	ret->Fill(WHITEA);
	
	ret->insertImage(sh1, 0, 0);
	ret->insertImage(sh2, sh1->getWidth() - 2, 0);

	
//	if (name)
	{
//		label = CRasterImage::CreateText(name + 6, 1, 20, 100, 18, 28, WHITEA);
//		ret->insertImage(label, 20, 5);
	}

	Delete(label);
	Delete(sh1);
	Delete(sh2);

	return AddLabel(name, ret);
}

#endif

extern bool bitPermutation(CCircuit::CShare* bits);

void CCircuit::OnNodeBuilded(position* p)
{
	u32 idx = p->getIndex();
	switch (m_mode)
	{
		case eAlways:
		{
			CSharesHolder& holder = *dynamic_cast<CSharesHolder*>(p->getElement());
			string str = format("share_G%d-L-%d", idx, 0);
			SerializeShare(*str, ConcatenateShare(*str, holder(0, 0)), true);
			str = format("share_G%d-L-%d", idx, 1);
			SerializeShare(*str, ConcatenateShare(*str, holder(1, 0)), true);
		/*	if (holder.IsInverted())
			{
				str = format("share_G%d-R-%d", idx, 0);
				SerializeShare(*str, ConcatenateShare(*str, holder(1, 1), holder(0, 1)), true);
				str = format("share_G%d-R-%d", idx, 1);
				SerializeShare(*str, ConcatenateShare(*str, holder(1, 2), holder(0, 2)), true);
			}
			else
			{*/
				str = format("share_G%d-R-%d", idx, 0);
				SerializeShare(*str, ConcatenateShare(*str, holder(0, 1), holder(1, 1)), true);
				str = format("share_G%d-R-%d", idx, 1);
				SerializeShare(*str, ConcatenateShare(*str, holder(0, 2), holder(1, 2)), true);
		//	}
			break;
		}
		default: break;
	}
	
	
}

// ogni bit
// ogni 8
// linee

void CCircuit::OnNodeSolved(position* p)
{
	switch (m_mode)
	{
		case eAlways:
		{
			CSharesHolder& holder = *dynamic_cast<CSharesHolder*>(p->getElement());
			string str = format("result_%d", p->getIndex());
			SerializeShare(*str, ConcatenateShare(*str, holder.GetResult()), true);
		}
		default: break;
	}
}

u32 CCircuit::GetInputsCount ()
{
	CVector<position*> leaves;
	m_tree.getLeaves(leaves, m_tree.root());
	return leaves.size();
}

namespace detail
{
	void dummy_callback(CircuitImageNode* nod) { }
	void dummy_start_callback(u32 w, u32 h) { }
}

OnDrawingFunction drawing_callback = detail::dummy_callback;
OnStartDrawingFunction start_drawing_callback = detail::dummy_start_callback;

extern "C" VGESS_API void SetDrawingCallback(OnDrawingFunction cb)
{
	drawing_callback = cb;
}

extern "C" VGESS_API void SetStartDrawingCallback(OnStartDrawingFunction cb)
{
	start_drawing_callback = cb;
}

void CCircuit::OnCircuitDrawn(CRasterImage* img, CCircuit::position* p, u32 x, u32 y)
{
	CircuitImageNode* node = New<CircuitImageNode>();
	node->gate = p ? p->getElement() : 0;
	node->img = img;
	node->x = x;
	node->y = y;
	drawing_callback(node);
}

u32 CCircuit::getLevels()
{
	m_tree.bfsVisit(SCountLayerBfs(m_tree_layers));
	return (++m_tree_layers);
}