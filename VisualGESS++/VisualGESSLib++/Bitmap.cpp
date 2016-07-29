/*
	file name: Bitmap.cpp
	author: Ventura Del Monte & Anna De Angelis
	created on: 15/04/13
*/

#include "stdafx.h"
#include "Bitmap.h"


CBitmap::CBitmap(u32 w, u32 h, u32 bits_color)	:
	m_width(w), m_height(h), m_bytes_per_pixel(bits_color / 8)
{
	m_data = New<CPixelMatrix>(w, h);
}

CBitmap::CBitmap() 
{
	CBitmap(0, 0, 0);
}

CBitmap::~CBitmap()
{
	Delete(m_data);
}

u32 CBitmap::getWidth() const
{
	return m_width;
}

u32 CBitmap::getHeight() const
{
	return m_height;
}

void CBitmap::Save(CWriter *fs)
{
	m_infos.width            = m_width;
	m_infos.height           = m_height;
	m_infos.bit_count        = u16(m_bytes_per_pixel << 3);
	m_infos.clr_important    = 0;
	m_infos.clr_used         = 0;
	m_infos.compression      = BI_RGB;
	m_infos.planes           = 1;
	m_infos.size             = sizeof(SBitmapInfo);
	m_infos.x_per_meter		 = 0x0ec4;
	m_infos.y_per_meter		 = 0x0ec4;
	m_infos.size_image       = m_infos.width * m_bytes_per_pixel * m_infos.height;

	m_header.type			= 0x4d42;
	m_header.size			= sizeof(SBitmapInfo) + sizeof(SBitmapHeader) + m_infos.size_image;
	m_header.reserved1		= 0;
	m_header.reserved2		= 0;
	m_header.ofs_bits		= 0x36;

	fs->Write<u16>(m_header.type);
	fs->Write<u32>(m_header.size);
	fs->Write<u16>(m_header.reserved1);
	fs->Write<u16>(m_header.reserved2);
	fs->Write<u32>(m_header.ofs_bits);

	fs->Write<u32>(m_infos.size);
	fs->Write<u32>(m_infos.width);
	fs->Write<u32>(m_infos.height);
	fs->Write<u16>(m_infos.planes);
	fs->Write<u16>(m_infos.bit_count);
	fs->Write<u32>(m_infos.compression);
	fs->Write<u32>(m_infos.size_image);
	fs->Write<u32>(m_infos.x_per_meter);
	fs->Write<u32>(m_infos.y_per_meter);
	fs->Write<u32>(m_infos.clr_used);
	fs->Write<u32>(m_infos.clr_important);

	u32 padding = (4 - ((3 * m_width) % 4)) % 4;
	char padding_data[4] = {0x0, 0x0, 0x0, 0x0};
	for (int i = 0; i < int(m_width); ++i)
	{
		for (int j = int(m_height - 1); j >= 0; --j)
		{
			SColor c = (*m_data)[i][j];
			if (m_bytes_per_pixel == 3)
			{
				fs->Write<u8>(c.b);
				fs->Write<u8>(c.g);
				fs->Write<u8>(c.r);
				fs->Write(padding_data, padding);
			}
			else if (m_bytes_per_pixel == 4)
			{
				fs->Write<u8>(c.b);
				fs->Write<u8>(c.g);
				fs->Write<u8>(c.r);
				fs->Write<u8>(c.a);
				fs->Write(padding_data, padding);
			}
			else
			{
				Error("bytes per pixel not valid");
			}
#ifdef LOGGING
			Log(" Writing @(%d,%d)=%08x", i, j, c.Get());
#endif
			
		}
	}
}

void CBitmap::Load(CReader *fs)
{

	m_header.type										= fs->Read<u16>();
	m_header.size										= fs->Read<u32>();
	m_header.reserved1									= fs->Read<u16>();
	m_header.reserved2									= fs->Read<u16>();
	m_header.ofs_bits									= fs->Read<u32>();

	m_infos.size										= fs->Read<u32>();
	m_width					=  m_infos.width            = fs->Read<u32>();
	m_height				=  m_infos.height           = fs->Read<u32>();
	m_infos.planes										= fs->Read<u16>();
	m_infos.bit_count									= fs->Read<u16>();
	m_bytes_per_pixel									= m_infos.bit_count / 8;
	m_infos.compression									= fs->Read<u32>();
	m_infos.size_image									= fs->Read<u32>();
	m_infos.x_per_meter									= fs->Read<u32>();
	m_infos.y_per_meter									= fs->Read<u32>();
	m_infos.clr_used									= fs->Read<u32>();
	m_infos.clr_important								= fs->Read<u32>();
	
	Delete(m_data);
	m_data = New<CPixelMatrix>(m_width, m_height);

	u32 padding = (4 - ((3 * m_width) % 4)) % 4;
	for (int i = 0; i < int(m_width); ++i)
	{
		for (int j = int(m_height - 1); j >= 0; --j)
		{
			SColor* c = &(*m_data)[i][j];
			if (m_bytes_per_pixel == 3)
			{
				c->b = fs->Read<u8>();
				c->g = fs->Read<u8>();
				c->r = fs->Read<u8>();
				if(padding)
					fs->Read<u8>();
			}
			else if (m_bytes_per_pixel == 4)
			{
				c->b = fs->Read<u8>();
				c->g = fs->Read<u8>();
				c->r = fs->Read<u8>();
				c->a = fs->Read<u8>();
				if(padding)
					fs->Read<u8>();  
			}
#ifdef LOGGING
			Log(" Reading @(%d,%d)=%08x", i, j, c->Get());	
#endif
		}
	}
}

void CBitmap::Set(u32 x, u32 y, SColor c)
{
#ifdef LOGGING
	Log(" Set @(%d,%d)=%08x", x, y, c.Get());
#endif
	(*m_data)[x][y] = c;
}

void CBitmap::Set(u32 x, u32 y, u32 c)
{
	(*m_data)[x][y] = SColor(c);
}

SColor CBitmap::Get(u32 x, u32 y) 
{
	return (*m_data)[x][y];
}

void CBitmap::setRegion(u32 x, u32 y, u32 w, u32 h, ELEMENT_TYPE* pixels, u32 dim)
{
	detail::SRegionFiller<ELEMENT_TYPE>::fillRegion(this, x, y, w, h, pixels, dim);
}

void CBitmap::setRegion(u32 x, u32 y, u32 w, u32 h, int* pixels, u32 dim)
{
	detail::SRegionFiller<ELEMENT_TYPE, int>::fillRegion(this, x, y, w, h, pixels, dim);
}

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

CBitmap* CBitmap::False(const u32 size)
{
	CBitmap* ret = New<CBitmap>(size, size);
	for (u32 i = 0; i < size; i++)
	{
		for (u32 j = 0; j < size; j++)
		{
			ret->Set(i, j, (j < size / 2) ? WHITE : BLACK);
		}
	}
	return ret;
}

CBitmap* CBitmap::True(const u32 size)
{
	CBitmap* ret = New<CBitmap>(size, size);
	for (u32 i = 0; i < size; i++)
	{
		for (u32 j = 0; j < size; j++)
		{
			ret->Set(i, j, (j >= size / 2) ? WHITE : BLACK);
		}
	}
	return ret;
}


