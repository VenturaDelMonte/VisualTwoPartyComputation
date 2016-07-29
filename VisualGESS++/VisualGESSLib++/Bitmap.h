/*
	file name: Bitmap.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 15/04/13
*/
#pragma once
#ifndef __BITMAP_H__
#define __BITMAP_H__

#define DEFAULT_IMAGE_SIZE 8

#include "Matrix.h"
#include "BaseImage.h"
#include "Color.h"
#include "Serializable.h"

class CWriter;
class CReader;

static const u32 DEFAULT_BITS_PER_PIXEL = 24;

class CBitmap : public ISerializable, public IPureBaseImage<SColor> 
{
	private:
		typedef Matrix<SColor> CPixelMatrix;
	private:
		struct SBitmapHeader
		{
			u16 type;
			u32 size;
			u16 reserved1;
			u16 reserved2;
			u32 ofs_bits;
		} m_header;
		struct SBitmapInfo
		{
			u32 size;
			u32 width;
			u32 height;
			u16 planes;
			u16 bit_count;
			u32 compression;
			u32 size_image;
			u32 x_per_meter;
			u32 y_per_meter;
			u32 clr_used;
			u32 clr_important;
		} m_infos;
		CPixelMatrix* m_data;
		u32 m_width, m_height;
		u16 m_bytes_per_pixel;
	public:
							CBitmap				(u32 w, u32 h, u32 bits_color = DEFAULT_BITS_PER_PIXEL);
							CBitmap				();
		virtual				~CBitmap			();

		virtual	void		Save				(CWriter *fs);
		virtual	void		Load				(CReader *fs);

		virtual	void		Set					(u32 x, u32 y, SColor c);
				void		Set					(u32 x, u32 y, u32 c);
				CBitmap*	Expand				(int x); 
		virtual	SColor		Get					(u32 x, u32 y);
		virtual	u32			getWidth			() const;
		virtual	u32			getHeight			() const;
		virtual void		setRegion			(u32 x, u32 y, u32 w, u32 h, ELEMENT_TYPE* pixels, u32 dim);
		virtual void		setRegion			(u32 x, u32 y, u32 w, u32 h, int* pixels, u32 dim);

		static	CBitmap*		False				(const u32 size = DEFAULT_IMAGE_SIZE);
		static  CBitmap*		True				(const u32 size = DEFAULT_IMAGE_SIZE);


};

#endif
