/*
	file name: color_list.cpp
	author: Ventura Del Monte & Anna De Angelis
	created on: 14/05/13
*/

#include "stdafx.h"
#include "color_list.h"
#include "color.h"


#define DEFAULT_BLACK_WHITE_COUNT 2

void CBlackWhiteColorList::Generate(int n, int m)
{
	ASSERT(m_colors_list == NULL && m_matrices_list.empty(), "list already initialized!");
	m_colors_list = ::MemHandler.Alloc<SColor>(DEFAULT_BLACK_WHITE_COUNT);
	m_colors_list[0] = WHITEA;
	m_colors_list[1] = BLACK;
	m_matrices_list[0] = New<IMatrix>(n, m);
	m_matrices_list[1] = New<IMatrix>(n, m);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			m_matrices_list[0]->setAt(i, j, (j == 0) ? WHITEA : BLACK); // j % 2 == 0
			m_matrices_list[1]->setAt(i, j, (j == i) ? WHITEA : BLACK);
		}
	}
}

CBlackWhiteColorList::~CBlackWhiteColorList()
{
	Delete(m_colors_list);
	//DeleteVector(m_matrices_list);
}

std::pair<SColor&, IMatrix*> CBlackWhiteColorList::operator[] (int i)
{
	ASSERT(m_colors_list != NULL && !m_matrices_list.empty(), "list not initialized!");
	ASSERT(i < DEFAULT_BLACK_WHITE_COUNT, "error out of bounds");
	return std::pair<SColor&, IMatrix*>(m_colors_list[i], m_matrices_list[i]);
}

bool CBlackWhiteColorList::isAllowed(u32 clr)
{
	return m_colors_list[0].Get() == clr || m_colors_list[1].Get() == clr;
}
