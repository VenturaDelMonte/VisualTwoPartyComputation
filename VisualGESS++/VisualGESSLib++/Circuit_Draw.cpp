#include "stdafx.h"
#include "Circuit.h"
#include "Gate.h"
#include "ResourceLoader.h"
#include "CircuitIO.h"

static const u16 SQUARES_SIZE = 80;

extern OnStartDrawingFunction start_drawing_callback;

struct SListLayerBfs
{
private:
	CAssocVector<u32, CVector<CCircuit::position*>>&	m_vec;
	u32&												m_max_layer;	
public:
	explicit SListLayerBfs(CAssocVector<u32, CVector<CCircuit::position*>>& vec, u32& max_layer) 
		: m_vec(vec), m_max_layer(max_layer)
	{
	}

	void operator()(u32 idx, CCircuit::position* pos) const
	{
		m_vec[idx].push_back(pos);
		m_max_layer = max(m_max_layer, idx);
	//	Log("%d %p", idx, pos);
	}

};

class CDrawingThread : public CThread
{
		
public:

	typedef CThread										base0;
	typedef CCircuit::CGateTree							CTree;
	typedef CTree::SBTreeNode							position;
	typedef fastdelegate::FastDelegate4<CRasterImage*, position*, u32, u32>	OnDrawingCallback;
	
	CDrawingThread(volatile CThreadPool* pool, OnDrawingCallback callback, CTree* tree) :
		base0(pool), m_callback(callback)
	{
		m_tree = tree;
		m_tree_layers = 0;
		m_img = 0;
	}

	virtual ~CDrawingThread()	
	{
		
	}

	virtual void Run() 
	{
		CAssocVector<u32, CVector<position*>> layers;
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
		m_tree->bfsVisit(SListLayerBfs(layers, m_tree_layers));
		const u32 size_y = SQUARES_SIZE + (m_tree_layers + 1) * 2 * SQUARES_SIZE;
		const u32 leaves = u32(std::pow(2, m_tree_layers));
		const u32 size_x = 2 * SQUARES_SIZE + 2 * SQUARES_SIZE * leaves + SQUARES_SIZE;
		CRasterImage::EImageType type = CRasterImage::eBitmapType;
		u16 depth = DEFAULT_BITS_PER_PIXEL;
		m_img = New<CRasterImage>(size_x, size_y, depth, type);
		m_img->Fill(WHITE);

		start_drawing_callback(size_x, size_y);

		m_total = m_tree->Size();
		m_current = 0;

		SGatesImages images;

		LoadGatesImages(&images, SQUARES_SIZE, SQUARES_SIZE);

		// first pass: draw the gates
		DrawGates(&images, m_img, m_tree->root(), 0, size_x - 2 * SQUARES_SIZE, 0);
		// second pass: draw the wires
		DrawWires(&images, m_img, m_tree->root(), 0, size_x - SQUARES_SIZE, SQUARES_SIZE, m_tree_layers);

	
		extern string g_output_folder;
		char circuit_name[100];
		sprintf(circuit_name, "%s\\circuit.bmp", *g_output_folder);
		CWriter* w = FS.w_open(circuit_name);
		m_img->Save(w);
		Delete(w);
		Delete(m_img);
	}

	

protected:

	void DrawGates(SGatesImages* images, CRasterImage* res, position* pos, u32 xl, u32 xr, u32 h)
	{
		u32 idx = pos->getIndex();
		IGate* g = pos->getElement();
		CRasterImage* img0 = 0;
		CRasterImage* img1 = 0;
		bool is_not = g->IsNegate();
		u32 x = xl + u32(floor((xr - xl) / 2)); // - floor(SQUARES_SIZE / 2));
		float q = float(x) / float(SQUARES_SIZE);
		x = (u32) ceil(q) * SQUARES_SIZE;
		u32 y = h + SQUARES_SIZE;
		//Log("su %d %d", xl, xr);
		bool output = false;
		char gate[64];
		sprintf(gate, "G%d", idx);
		switch (g->GetType())
		{
			case IGate::eAndGate:	
			{
				img0 = (is_not) ? images->m_nandPort : images->m_andPort;
				img1 = CRasterImage::CreateText(gate, 20, 60, SQUARES_SIZE, SQUARES_SIZE, 36, WHITEA);
				break;
			}
			case IGate::eOrGate:	
			{				
				img0 = (is_not) ? images->m_norPort : images->m_orPort;		
				img1 = CRasterImage::CreateText(gate, 20, 60, SQUARES_SIZE, SQUARES_SIZE, 36, WHITEA);
				break;
			}
			default:				
			{
				//img0 = CRasterImage::CreateText("annna", 10, 10, SQUARES_SIZE, SQUARES_SIZE);
				char port[64];
				output = true; 
				sprintf(port, "x%d", dynamic_cast<CInputGate*>(g)->GetPortNumber());
				img0 = CRasterImage::CreateText(port, 25, 70, SQUARES_SIZE, SQUARES_SIZE, 36, WHITE);
				break;
			}
		}
		res->insertImage(img0, x, y);
		CRasterImage* r = CRasterImage::Clone(img0);
		if (img1)
		{
			for(u32 i = 0; i < img1->getWidth(); i++)
			{
				for(u32 j = 0; j < img1->getHeight(); j++)
				{
					SColor p = img1->Get(i, j);
					if (p == BLACK)
					{
						res->Set(i + x, y + j, p);
						r->Set(i, j, p);
					}
				}
			}
		}
		
		m_callback(r, pos, x, y);
		m_current++;
		m_progress = float(m_current / m_total);
		Sleep(50);
		if (m_tree->hasLeft(pos))
			DrawGates(images, res, m_tree->left(pos), xl, x, y + SQUARES_SIZE);
		if (m_tree->hasRight(pos))
			DrawGates(images, res, m_tree->right(pos), x, xr, y + SQUARES_SIZE);
	
		if (output)
			Delete(img0);
	
	}


	void DrawWires(SGatesImages* images, CRasterImage* res, position* pos, u32 xl, u32 xr, u32 h, u32 level) 
	{
		u32 x = xl + u32(floor((xr - xl) / 2)); // - u32(floor(SQUARES_SIZE / 2));
		u32 y = h + SQUARES_SIZE;
		float q = float(x) / float(SQUARES_SIZE);
		x = (u32) ceil(q) * SQUARES_SIZE;
		u32 mul = u32(std::pow(2, level) / 2);
		if (m_tree->isRoot(pos))
			xr -= SQUARES_SIZE;
		if (m_tree->isExternal(pos)) 
			return;
		res->insertImage(images->m_topWire, x, y);
		m_callback(CRasterImage::Clone(images->m_topWire), 0, x, y);
		for (u32 i = 1; i <= mul; i++)
		{
			res->insertImage(images->m_Wire, x + i * SQUARES_SIZE, y);
			m_callback(CRasterImage::Clone(images->m_Wire), 0, x + i * SQUARES_SIZE, y);
			res->insertImage(images->m_Wire, x - i * SQUARES_SIZE, y);
			m_callback(CRasterImage::Clone(images->m_Wire), 0, x - i * SQUARES_SIZE, y);
		}
		res->insertImage(images->m_leftWire, x + mul * SQUARES_SIZE, y);
		res->insertImage(images->m_rightWire, x - mul * SQUARES_SIZE, y);
		m_callback(CRasterImage::Clone(images->m_leftWire), 0, x + mul * SQUARES_SIZE, y);
		m_callback(CRasterImage::Clone(images->m_rightWire), 0, x - mul * SQUARES_SIZE, y);
		Sleep(50);
		if (m_tree->hasLeft(pos))
			DrawWires(images, res, m_tree->left(pos), xl, x, y + SQUARES_SIZE, level - 1);
		if (m_tree->hasRight(pos))
			DrawWires(images, res, m_tree->right(pos), x, xr, y + SQUARES_SIZE, level - 1);

	}


private:

	CTree* m_tree;
	CRasterImage* m_img;
	u32 m_total, m_current, m_tree_layers;
	OnDrawingCallback m_callback;
};

CThread* CCircuit::Draw(CThreadPool* pool)
{
	
/*
	u32 start_x = SQUARES_SIZE, start_y = SQUARES_SIZE;
	u32 factor_x = 1;
	for (s32 i = layers_count; i >= 0; i--)
	{
		// first pass: draw the gates
		u32 gates_count = layers[i].size();
		for (u32 j = 0; j < gates_count; j++)
		{
			CBitmapEx* img0 = 0;
			IGate* g = layers[i][j]->getElement();
			bool is_not = g->IsNegate();
			u32 x = start_x + 2 * SQUARES_SIZE * j * factor_x;
			u32 y = start_y + 2 * SQUARES_SIZE * (layers_count - i);
			if (!i)
				x = SQUARES_SIZE * leaves; // correzione root wtf!?
			switch (g->GetType())
			{
				case IGate::eAndGate:	img0 = (is_not) ? images.m_nandPort : images.m_andPort;		break;
				case IGate::eOrGate:	img0 = (is_not) ? images.m_norPort : images.m_orPort;		break;
				default:				img0 = NULL;												break;
			}
			if (!img0) continue;
			img->insertImage(img0, x, y);
		}
		start_x += (SQUARES_SIZE * (1 + layers_count - i)); 
		factor_x = (layers_count - i + 1) * 2;
		// second pass: draw the wires
	}
*/
	CGateTree* tree = &m_tree;
	CDrawingThread* t = New<CDrawingThread>(pool, CDrawingThread::OnDrawingCallback(this, &CCircuit::OnCircuitDrawn), tree);
	return t;
}