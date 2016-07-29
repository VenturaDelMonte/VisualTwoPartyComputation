/*
	file name: MVCSEncrypter_impl.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 14/05/13
*/
#pragma once

#define TEMPLATE_DEF		template <class ImageType, u16 MAX_INPUT>
#define CSMVCSEncrypter		CMVCSEncrypter<ImageType, MAX_INPUT>

TEMPLATE_DEF
CSMVCSEncrypter::CMVCSEncrypter(IColorList* colors, u16 k, u16 n, u16 m, u16 h, u16 v) :
	CVCSEncrypter(colors, k, n, m, h, v)
{
	u32 val = u32(GetQueryPerformaceCounter() & u32(-1));
	SetSeed(val);
	SetPixelExpansion(1, 1); // force no pixel expansion
}

TEMPLATE_DEF
CSMVCSEncrypter::~CMVCSEncrypter()
{
}

TEMPLATE_DEF 
void CSMVCSEncrypter::Encrypt(IMAGE_STORAGE& res)
{
	ASSERT(m_input.size() == MAX_INPUT, "not enough or too much input images");

	__super::Encrypt(res);

	CImageType* img1 = m_input[1]; ASSERT(img1, "invalid img1");

	const u32 width = img1->getWidth(), height = img1->getHeight();
	const u32 new_width = width * H, new_height = height * V; // vabbe non ci sta PE
	
	CImageType* share1 = res[0];

	res.push_back(New<CImageType>(new_width, new_height)); 

	CImageType* share3 = res[2];

	for (u32 i = 0; i < width; ++i)
	{
		for (u32 j = 0; j < height; ++j)
		{
			CImageType::ELEMENT_TYPE pixel = img1->Get(i, j);
			if (pixel == BLACK)
			{
				if (share1->Get(i, j) == BLACK) 
					share3->Set(i, j, WHITEA);
				else 
					share3->Set(i, j, BLACK);
			}
			else if (pixel == WHITEA)
			{
				if (share1->Get(i, j) == BLACK) 
					share3->Set(i, j, BLACK);
				else 
					share3->Set(i, j, WHITEA);
			}
		}
	}

#ifdef DEBUG
	for (u32 i = 0; i < width; ++i)
	{
		for (u32 j = 0; j < height; ++j)
		{
			CImageType::ELEMENT_TYPE p0 = m_input[0]->Get(i, j);
			CImageType::ELEMENT_TYPE p1 = m_input[1]->Get(i, j);
			if (p0 == BLACK)
			{
				CImageType::ELEMENT_TYPE c0 = res[0]->Get(i, j);
				CImageType::ELEMENT_TYPE c1 = res[1]->Get(i, j);
				ASSERT((c0 + c1) == p0, "wrong color");
			}
			if (p1 == BLACK)
			{
				CImageType::ELEMENT_TYPE c0 = res[0]->Get(i, j);
				CImageType::ELEMENT_TYPE c2 = res[2]->Get(i, j);
				ASSERT((c0 + c2) == p1, "wrong color");
			}

		}
	}
#endif
	
}

#undef TEMPLATE_DEF
#undef CSMVCSEncrypter
