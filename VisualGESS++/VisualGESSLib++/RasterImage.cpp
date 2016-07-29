/*
	file name: BitMap.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 30/04/13
*/
#include "stdafx.h"
#include "RasterImage.h"
#include "Color.h"


CRasterImage::CRasterImage(u32 w, u32 h, u16 bits_per_pixel, EImageType image_format) :
		m_data(w, h, bits_per_pixel, image_format), m_format(image_format)
{
	m_data.AlphaCreate();
#if 1
	for (u32 i = 0; i < w; i++)
	{
		for (u32 j = 0; j < h; j++)
		{
			Set(i, j, WHITEA);
		}
	}
#endif
	
}

CRasterImage::CRasterImage() 
{
	CRasterImage(1, 1, DEFAULT_BITS_PER_PIXEL, eBitmapType);
}

CRasterImage::~CRasterImage()
{
	Destroy();
}

void CRasterImage::Destroy()
{
	m_data.AlphaDelete();
	m_data.DestroyFrames();
	m_data.Destroy();
}

u32 CRasterImage::getWidth() const
{
	return m_data.GetWidth();
}

u32 CRasterImage::getHeight() const
{
	return m_data.GetHeight();
}

void CRasterImage::Save(CWriter *fs)
{
	u8* buffer = 0;
	s32 size = 0;
	ASSERT(!!m_data.Encode(buffer, size, m_format), *format("error during saving image %s", m_data.GetLastError()));
	fs->Write(buffer, size);
	m_data.FreeMemory(buffer);
}

void CRasterImage::Save(LPCSTR file_name)
{
	CWriter* w = FS.w_open(file_name);
	Save(w);
	Delete(w);
}

void CRasterImage::Load(CReader *fs)
{
	ASSERT(!!m_data.Decode(fs->Pointer(), fs->Size(), m_format), *format("error during loading image %s", m_data.GetLastError()));
}

void CRasterImage::Load(HBITMAP hObject)
{
	Destroy();
	ASSERT(!!m_data.CreateFromHBITMAP(hObject), *format("error during loading image %s", m_data.GetLastError()));
}

void CRasterImage::Load(FILE *fp)
{
	m_data.Decode(fp, m_format);
}

void CRasterImage::Save(FILE *fp)
{
	m_data.Encode(fp, m_format);
	fclose(fp);
}

void CRasterImage::Set(u32 x, u32 y, SColor c)
{
#ifdef LOGGING
	Log(" Set @(%d,%d)=%08x", x, y, c.Get());
#endif
	m_data.SetPixelColor(x, y, c, true);
}

void CRasterImage::Set(u32 x, u32 y, u32 c)
{
	Set(x, y, SColor(c));
}

SColor CRasterImage::Get(u32 x, u32 y) 
{
	return SColor(m_data.GetPixelColor(x, y, true));
}

void CRasterImage::setRegion(u32 x, u32 y, u32 w, u32 h, ELEMENT_TYPE* pixels, u32 dim)
{
	detail::SRegionFiller<ELEMENT_TYPE>::fillRegion(this, x, y, w, h, pixels, dim);
}

void CRasterImage::setRegion(u32 x, u32 y, u32 w, u32 h, int* pixels, u32 dim)
{
	detail::SRegionFiller<ELEMENT_TYPE, int>::fillRegion(this, x, y, w, h, pixels, dim);
}

void CRasterImage::SetFormat(EImageType format)
{
	m_format = format;
}

CRasterImage::EImageType CRasterImage::GetFormat() const
{
	return m_format;
}

CRasterImage* CRasterImage::Expand(u32 expansion)
{
	u32 w = m_data.GetWidth();
	u32 h = m_data.GetHeight();
	int new_w = w * expansion;
	int new_h = h * expansion;
	CRasterImage* bmp_E = New<CRasterImage>(new_w, new_h);
	for (u32 i = 0; i < w; i++)
	{
		for (u32 j = 0; j < h; j++)
		{
			SColor c = Get(i, j);
			for (u32 ii = i * expansion, l = (i + 1) * expansion; ii < l; ii++)
			{
				for (u32 jj = j * expansion, ll = (j + 1) * expansion ; jj < ll; jj++)
				{
					bmp_E->Set(ii, jj, c);
				}
			}
		}
	}
	return bmp_E;
}

CRasterImage* CRasterImage::ExpandGrid(u32 expansion, SColor c)
{
	u32 w = m_data.GetWidth();
	u32 h = m_data.GetHeight();
	u32 new_w = w * expansion + w + 1;
	u32 new_h = h * expansion + h + 1;
	u32 total_cells = w * h;
	CRasterImage* bmp_E = New<CRasterImage>(new_w, new_h);
	
	bmp_E->Fill(c);

	for (u32 i = 0; i < w; i++)
	{
		for (u32 j = 0; j < h; j++)
		{
			u32 rx = i * (1 + expansion) + 1;
			u32 ry = j * (1 + expansion) + 1;
			SColor p = Get(i, j);	
			for (u32 ii = 0; ii < expansion; ii++)
			{
				for (u32 jj = 0; jj < expansion; jj++)
				{
					bmp_E->Set(rx + ii, ry + jj, p);
				}
			}			
		}
	}
	
	return bmp_E;
}


CRasterImage* CRasterImage::False(const u32 size)
{
	CRasterImage* ret = New<CRasterImage>(size, size);
	for (u32 j = 0; j < size; j++)
	{
		for (u32 i = 0; i < size; i++)
		{
			//ret->Set(j, i, (j >= size / 2) ? WHITE : BLACK);
			ret->Set(j, i, WHITEA);
			//ret->Set(j, i, (i >= size / 2) ? WHITE : BLACK);
		}
	}
	return ret;
}

CRasterImage* CRasterImage::True(const u32 size)
{
	CRasterImage* ret = New<CRasterImage>(size, size);
	for (u32 i = 0; i < size; i++)
	{
		for (u32 j = 0; j < size; j++)
		{
		//	ret->Set(j, i, (j < size / 2) ? WHITE : BLACK);
			ret->Set(j, i, BLACK);
		//	ret->Set(j, i, (i < size / 2) ? WHITE : BLACK);
		}
	}
	return ret;
}

CRasterImage* CRasterImage::BitFalse(bool a)
{
	const u32 size_x = 2, size_y = 1;
	CRasterImage* ret = New<CRasterImage>(size_x, size_y);
	if (!a)
	{
		ret->Set(0, 0, BLACK);
		ret->Set(1, 0, WHITEA);
	}
	else
	{
		ret->Set(0, 0, WHITEA);
		ret->Set(1, 0, BLACK);
	}
	return ret;
}

CRasterImage* CRasterImage::BitTrue()
{
	const u32 size_x = 2, size_y = 1;
	CRasterImage* ret = New<CRasterImage>(size_x, size_y);
	ret->Set(0, 0, BLACK);
	ret->Set(1, 0, BLACK);
	return ret;
}

CRasterImage* CRasterImage::Join(CRasterImage* b1, CRasterImage* b2)
{
//	ASSERT(b1 && b2, "invalid input");
	if (!b1 && b2)
		return Clone(b2);
	if (!b2 && b1)
		return Clone(b1);
	ASSERT(b1 && b2, "invalid input");
	u32 b1_y = b1->getHeight();
	u32 b1_x = b1->getWidth();
	
	u32 b2_y = b2->getHeight();
	u32 b2_x = b2->getWidth();

	const u32 size_x = b2_x + b1_x, size_y = b2_y;
	CRasterImage* ret = New<CRasterImage>(size_x, size_y);

	for (u32 i = 0; i < size_x; i++)
	{
		for (u32 j = 0; j < size_y; j++)
		{
			ret->Set(i, j, (i >= b1_x) ? b2->Get(i - b1_x, j) : b1->Get(i, j));
		}
	}

	return ret;
}

void CRasterImage::Split(CRasterImage* b, CRasterImage* result1, CRasterImage* result2)
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

CRasterImage* CRasterImage::Clone(CRasterImage* img)
{
	if (img == NULL) 
		return NULL;
	const u32 depth = DEFAULT_BITS_PER_PIXEL;
	EImageType format = img->m_format;
	const u32 size_x = img->getWidth(), size_y = img->getHeight();
	CRasterImage* ret = New<CRasterImage>(size_x, size_y, depth, format);
	for (u32 i = 0; i < size_x; i++)
	{
		for (u32 j = 0; j < size_y; j++)
		{
			ret->Set(i, j, img->Get(i, j));
		}
	}
	return ret;
}

CRasterImage* CRasterImage::Invert()
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

CRasterImage* CRasterImage::Fill(SColor c)
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

CRasterImage* CRasterImage::Repeat(CRasterImage* img, int a, bool destroy)
{
	if (a <= 1)
		return Clone(img);
	//u32 size_x = img->getWidth(), size_y = img->getHeight();
	CRasterImage *ret = 0, *tmp = Clone(img); //New<CRasterImage>(size_x, size_y);
	
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

CRasterImage* CRasterImage::insertImage(CRasterImage* img, u32 off_x, u32 off_y)
{
	ASSERT(img, "image is null");
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

CRasterImage* CRasterImage::insertImage(CRasterImage* img, u32 off_x, u32 off_y, u32 w, u32 h, u32 start_x, u32 start_y)
{
	ASSERT(img && w && h, "image is null");
	ASSERT((off_x <= getWidth() && off_y <= getHeight()), "error")
	
	for(u32 i = start_x; i < w; i++)
	{
		for(u32 j = start_y; j < h; j++)
		{
			Set(i + off_x, off_y + j, img->Get(i, j));
		}
	}
	
	return this;
}


CRasterImage* CRasterImage::CreateText(LPCSTR text, u32 x, u32 y, u32 w, u32 h, u32 s, SColor background)
{
	LOGFONT system_font;
	CRasterImage* ret = New<CRasterImage>(w, h);
	ret->Fill(background);
	//ret->Fill(WHITE);
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &system_font);
	ret->m_data.DrawString(GetDC(NULL), x, y, text, BLACK, system_font.lfFaceName, s, 400, 0, 0, true);
	return ret;

}

CRasterImage* CRasterImage::SubImage(u32 start_x, u32 start_y, u32 w, u32 h)
{
	CRasterImage* ret = New<CRasterImage>(w, h);
	if ((start_x > getWidth() - 1) || (start_y > getHeight()) || (start_x + w > getWidth()) || (start_y + h > getHeight()))
		return NULL;
	for (u32 x = start_x, xx = 0; xx < w; x++, xx++)
	{
		for (u32 y = start_y, yy = 0; yy < h; y++, yy++)
		{
			ret->Set(xx, yy, Get(x, y));
		}
	}
	return ret;
}