/*
	file name: VCSEncrypter.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 14/05/13
*/
#pragma once
#ifndef __VCSENCRYPTER_H__
#define __VCSENCRYPTER_H__

#include "PureEncrypter.h"
#include "Matrix.h"

class IColorList;

template 
<
	class ImageType = CBitmap,
	u16 MAX_INPUT	= 1
>
class CVCSEncrypter : public IPureEncrypter<ImageType>, public CRandom
{
	public:
		typedef IPureEncrypter<ImageType>		Base;
		typedef typename Base::CImageType		CImageType;
		typedef typename Base::IMAGE_STORAGE	IMAGE_STORAGE;
		DEFINE_ASSOC_VECTOR(MATRICES_STORAGE, u32, IMatrix*);
	public:
		explicit							CVCSEncrypter				(IColorList* colors, u16 k = 2, u16 n = 2, u16 m = 2, u16 h = 2, u16 v = 1);
		virtual								~CVCSEncrypter				();

		virtual		void					Encrypt						(IMAGE_STORAGE& st, IMAGE_STORAGE& res);
		virtual		void					Encrypt						(IMAGE_STORAGE& res);
		virtual		IPureEncrypter&			AddImage					(CImageType* img)				{ m_input.push_back(img); return *this; }

		virtual		IPureEncrypter&			SetPixelExpansion			(u16 h, u16 v)					{ H = h; V = v; return *this; }
	
	protected:
		IMAGE_STORAGE				m_input;
		s32*						m_shuffled;
		MATRICES_STORAGE			m_permuted_matrices;
		u16							K, N, M, H, V;
		IColorList*					m_colors;

	protected:
					IMatrix*		NextPermutation						(u32 clr);	
	//				IMatrix*		NextPermutation						(bool clr);
				
					
			
};

#include "VCSEncrypter_impl.h"




#endif
