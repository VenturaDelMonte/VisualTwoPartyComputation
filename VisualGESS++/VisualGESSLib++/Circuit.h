/*
	file name: Circuit.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 11/04/2013
*/
#pragma once
#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

#include "LinkedTree.h"
#include "TreeBuilder.h"
#include "RasterImage.h"

class IGate;
class CThread;
class CThreadPool;

#define DEFAULT_SHARE_SIZE 8
#define DEFAULT_SHARE_EXP 16

class VGESS_API CCircuit : public STreeBuilder<CRasterImage>
{
	
	public:		
		typedef CRasterImage									CShare;
		typedef IGate											IPureGate;

		typedef CLinkedBinaryTree<IPureGate*>					CGateTree;
		typedef CGateTree::SBTreeNode							position;

		typedef ISharesHolder<CShare>							CSharesHolder;
		typedef CSharesHolder::SharePair						CSharePair;


	public:
		inline								CCircuit			(u32 share_size = DEFAULT_SHARE_SIZE, ESerializationMode mode = eNever) 
																	: STreeBuilder(&m_tree), m_share_size(share_size), m_mode(mode), m_expansion(DEFAULT_SHARE_EXP)	 { Init(); 	}
		virtual								~CCircuit			();											


				void						Init				();
		//		void						BuildCallback		(s16 op);
				void						BuildShares			();
				
				CShare*						Solve				(CAssocVector<u32, bool>& input);

				CThread*					Draw				(CThreadPool* pool);

				void						GetChildren			(CVector<position*>& vec, position* g);

				void						OnNodeBuilded		(position* p);
				void						OnNodeSolved		(position* p);
				void						OnCircuitDrawn		(CRasterImage* img, position* p, u32 x, u32 y);

				u32							GetInputsCount		();
				void						SetExpansion		(u32 exp)										{ m_expansion = exp; }
	private:
	//			void 						SetPositions		(IPureGate* g, position *p);
				void						SetInput			(CAssocVector<u32, bool>& input);
	public:
				CShare*						ConcatenateShare	(LPCSTR name, CSharePair& p);
				CShare*						ConcatenateShare	(LPCSTR name, CSharePair& p1, CSharePair& p2);
	
		static  CShare*						ConcatenateShare	(LPCSTR name, CSharePair& p, u32 expansion, u32 shares_size);
		static	CShare*						ConcatenateShare	(LPCSTR name, CSharePair& p1, CSharePair& p2, u32 expansion, u32 shares_size);

				u32							getLevels			();

				u32							GetShareSize		() const { return m_share_size; }
				CGateTree&					GetTree				()       { return m_tree;		}

	private:
		CGateTree				m_tree;
		u32						m_share_size, m_expansion;
		ESerializationMode		m_mode;
public:	u32 m_tree_layers;	

};

#endif