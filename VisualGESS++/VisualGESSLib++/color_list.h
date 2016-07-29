/*
	file name: color_list.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 14/05/13
*/

#pragma once
#ifndef __COLOR_LIST_H__
#define __COLOR_LIST_H__

#include "Matrix.h"

struct SColor;


class ABSTRACT IColorList
{
	public:
		virtual													~IColorList					()					{}
		virtual			void									Generate					(int n, int m)		= 0;
		virtual			std::pair<SColor&, IMatrix*>			operator[]					(int i)				= 0;
		virtual			bool									isAllowed					(u32 clr)		    = 0;
};

class CBlackWhiteColorList : public IColorList
{
	public:
						CBlackWhiteColorList												(int n, int m)		: 	m_colors_list(0)					{ Generate(n, m); }
						CBlackWhiteColorList												()					: 	m_colors_list(0)					{  }
		virtual													~CBlackWhiteColorList		();
		virtual			std::pair<SColor&, IMatrix*>			operator[]					(int i);
		virtual			void									Generate					(int n, int m);
		virtual			bool									isAllowed					(u32 clr);
	private:
		SColor*				m_colors_list;
		CAssocVector<u32, IMatrix*>	m_matrices_list;
};

#endif