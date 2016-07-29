/*
	file name: BitMap.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 30/04/13
*/
#include "stdafx.h"
//.#include "BitmapEx.h"
#include "Color.h"
#include "fcntl.h"

CBitmapEx::CBitmapEx(u32 w, u32 h, u16 bytes_per_pixel)	
{
	m_data.assign(cimg_library::CImg<u8>(w, h, 1, bytes_per_pixel));
#ifdef DEBUG
	for (u32 i = 0; i < w; i++)
	{
		for (u32 j = 0; j < h; j++)
		{
			Set(i, j, SColor(255, 0, 0, 255));
		}
	}
#endif
}

CBitmapEx::CBitmapEx() 
{
	CBitmapEx(0, 0, 3);
}

CBitmapEx::~CBitmapEx()
{
	m_data.clear();
}


void CBitmapEx::Save(CWriter *fs)
{
	FILE* fp = fs->Handle();
	m_data.save_bmp(fp);
}

void CBitmapEx::Load(CReader *fs)
{
//	int fd = _open_osfhandle((long) fs->Handle(), O_RDONLY);
	FILE* fp = fs->Handle(); //fdopen(fd, "r");
	m_data.load_bmp(fp);
}

void CBitmapEx::LoadInternal(FILE *fp)
{
	m_data.load_bmp(fp);
}

void CBitmapEx::Set(u32 x, u32 y, SColor c)
{
#ifdef LOGGING
	Log(" Set @(%d,%d)=%08x", x, y, c.Get());
#endif
	int off_r = m_data.offset(x, y, 0, 0);
	int off_g = m_data.offset(x, y, 0, 1);
	int off_b = m_data.offset(x, y, 0, 2);
	m_data[off_r] = c.r;
	m_data[off_g] = c.g;
	m_data[off_b] = c.b;
}

void CBitmapEx::Set(u32 x, u32 y, u32 c)
{
	Set(x, y, SColor(c));
}

SColor CBitmapEx::Get(u32 x, u32 y) 
{
	return SColor(m_data(x, y, 0, 0), m_data(x, y, 0, 1), m_data(x, y, 0, 2), 255);
}

void CBitmapEx::setRegion(u32 x, u32 y, u32 w, u32 h, ELEMENT_TYPE* pixels, u32 dim)
{
	detail::SRegionFiller<ELEMENT_TYPE>::fillRegion(this, x, y, w, h, pixels, dim);
}

void CBitmapEx::setRegion(u32 x, u32 y, u32 w, u32 h, int* pixels, u32 dim)
{
	detail::SRegionFiller<ELEMENT_TYPE, int>::fillRegion(this, x, y, w, h, pixels, dim);
}
/*
CBitmap* CBitmap::Expand(int x)
{
	int new_w = m_width*x;
	int new_h = m_height*x;
	CBitmap* bmp_E = New<CBitmap>(new_w, new_h);
	for(u32 i = 0; i < m_width; i++)
	{
		for(u32 j = 0; j < m_height; j++)
		{
			SColor c =  Get(i, j);
			for(u32 ii = i, l = (i+1)*x; ii < l; ii++)
			{
				for(u32 jj = j, ll = (j+1)*x ; jj < ll; jj++)
				{
					bmp_E->Set(ii, jj, c);
				}
			}
		}
	}
	return bmp_E;
}
*/
CBitmapEx* CBitmapEx::False(const u32 size)
{
	CBitmapEx* ret = New<CBitmapEx>(size, size);
	for (u32 j = 0; j < size; j++)
	{
		for (u32 i = 0; i < size; i++)
		{
	//		ret->Set(j, i, (j >= size / 2) ? WHITE : BLACK);
			ret->Set(j, i, WHITE);
	//		ret->Set(j, i, (i >= size / 2) ? WHITE : BLACK);
		}
	}
	return ret;
}

CBitmapEx* CBitmapEx::True(const u32 size)
{
	CBitmapEx* ret = New<CBitmapEx>(size, size);
	for (u32 i = 0; i < size; i++)
	{
		for (u32 j = 0; j < size; j++)
		{
	//		ret->Set(j, i, (j < size / 2) ? WHITE : BLACK);
			ret->Set(j, i, BLACK);
		//	ret->Set(j, i, (i < size / 2) ? WHITE : BLACK);
		}
	}
	return ret;
}

CBitmapEx* CBitmapEx::BitFalse(bool a)
{
	const u32 size_x = 2, size_y = 1;
	CBitmapEx* ret = New<CBitmapEx>(size_x, size_y);
	if (!a)
	{
		ret->Set(0, 0, BLACK);
		ret->Set(1, 0, WHITE);
	}
	else
	{
		ret->Set(0, 0, WHITE);
		ret->Set(1, 0, BLACK);
	}
	return ret;
}

CBitmapEx* CBitmapEx::BitTrue()
{
	const u32 size_x = 2, size_y = 1;
	CBitmapEx* ret = New<CBitmapEx>(size_x, size_y);
	ret->Set(0, 0, BLACK);
	ret->Set(1, 0, BLACK);
	return ret;
}

CBitmapEx* CBitmapEx::Join(CBitmapEx* b1, CBitmapEx* b2)
{
	ASSERT(b1 && b2, "invalid input");
	u32 b1_y = b1->getHeight();
	u32 b1_x = b1->getWidth();
	
	u32 b2_y = b2->getHeight();
	u32 b2_x = b2->getWidth();

	const u32 size_x = b2_x + b1_x, size_y = b2_y;
	CBitmapEx* ret = New<CBitmapEx>(size_x, size_y);

	for (u32 i = 0; i < size_x; i++)
	{
		for (u32 j = 0; j < size_y; j++)
		{
			ret->Set(i, j, (i >= b1_x) ? b2->Get(i - b1_x, j) : b1->Get(i, j));
		}
	}

	return ret;
}

void CBitmapEx::Split(CBitmapEx* b, CBitmapEx* result1, CBitmapEx* result2)
{
	u32 b_y = b->getHeight();
	u32 b_x = b->getWidth();
	
	u32 res1_x = b_x / 2;
	
	
	for (u32 i = 0; i < b_x; i++)
	{
		for (u32 j = 0; j < b_y; j++)
		{
			if(i >= res1_x)
			{
				result2->Set(i - res1_x, j, b->Get(i, j));
			}
			else
				result1->Set(i, j, b->Get(i, j));
		}
	}
}

CBitmapEx* CBitmapEx::Clone(CBitmapEx* img)
{
	if (img == NULL) 
		return NULL;
	const u32 size_x = img->getWidth(), size_y = img->getHeight();
	CBitmapEx* ret = New<CBitmapEx>(size_x, size_y);
	for (u32 i = 0; i < size_x; i++)
	{
		for (u32 j = 0; j < size_y; j++)
		{
			ret->Set(i, j, img->Get(i, j));
		}
	}
	return ret;
}

CBitmapEx* CBitmapEx::Invert()
{
	for (u32 i = 0; i < getWidth(); i++)
	{
		for (u32 j = 0; j < getHeight(); j++)
		{
			Set(i, j, Get(i, j).Invert());
		}
	}
	return this;
}

CBitmapEx* CBitmapEx::Fill(SColor c)
{
	for (u32 i = 0; i < getWidth(); i++)
	{
		for (u32 j = 0; j < getHeight(); j++)
		{
			Set(i, j, c);
		}
	}
	return this;
}

CBitmapEx* CBitmapEx::Repeat(CBitmapEx* img, int a, bool destroy)
{
	if (a <= 1)
		return Clone(img);
	//u32 size_x = img->getWidth(), size_y = img->getHeight();
	CBitmapEx *ret = 0, *tmp = Clone(img); //New<CBitmapEx>(size_x, size_y);
	
	for (int i = 0; i < a - 1; i++)
	{
		ret = Join(tmp, img);
		Delete(tmp);
		tmp = ret;
	}
	if (destroy)
		Delete(img);
	return ret;
}

CBitmapEx* CBitmapEx::insertImage(CBitmapEx* img, u32 off_x, u32 off_y)
{
	ASSERT(img,"image null");
	ASSERT((off_x <= getWidth() && off_y <= getHeight()), "error")
	
	for(u32 i = 0; i < img->getWidth(); i++)
	{
		for(u32 j = 0; j < img->getHeight(); j++)
		{
			Set(i + off_x, off_y + j, img->Get(i, j));
		}
	}
	
	return this;
}
