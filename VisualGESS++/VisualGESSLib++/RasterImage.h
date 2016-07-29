/*
	file name: BitMap.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 30/04/13
*/
#pragma once
#ifndef __BITMAPEX_H__
#define __BITMAPEX_H__

#include "Serializable.h"
#include "BaseImage.h"
#include "ximage.h"

static const u32 DEFAULT_IMAGE_SIZE = 8;
static const u32 DEFAULT_BITS_PER_PIXEL = 24;

class VGESS_API CRasterImage : public ISerializable, public IPureBaseImage<SColor> 
{
	public:
		enum EImageType
		{
			eBitmapType		= CXIMAGE_FORMAT_BMP,
			eJpegType		= CXIMAGE_FORMAT_JPG,
			ePngType		= CXIMAGE_FORMAT_PNG,

			eInvalidType	= u32(-1),
		};
	public:
										CRasterImage				();
										CRasterImage				(u32 width, u32 height, u16 bits_per_pixel = DEFAULT_BITS_PER_PIXEL, EImageType image_format = ePngType);
			virtual						~CRasterImage				();

			virtual	void				Save						(CWriter *fs);
			virtual	void				Load						(CReader *fs);

					void				Load						(FILE* fp);
					void				Save						(FILE *fp);

					void				Save						(LPCSTR file_name);
							
			virtual	u32					getWidth					() const;												
			virtual	u32					getHeight					() const;												

			virtual	SColor				Get							(u32 i, u32 j);
			virtual	void				Set							(u32 i, u32 j, SColor val);
					void				Set							(u32 i, u32 j, u32 val);

					void				SetFormat					(EImageType format);
					EImageType			GetFormat					() const;

			virtual	void				setRegion					(u32 x, u32 y, u32 w, u32 h, SColor* pixels, u32 dim);
			virtual	void				setRegion					(u32 x, u32 y, u32 w, u32 h, int* pixels, u32 dim);

					CRasterImage*		Fill						(SColor c);

					CRasterImage*		insertImage					(CRasterImage* img, u32 off_x, u32 off_y);

					CRasterImage*		insertImage					(CRasterImage* img, u32 off_x, u32 off_y, u32 w, u32 h, u32 start_x, u32 start_y);

			static	CRasterImage*		False						(const u32 size = DEFAULT_IMAGE_SIZE);
			static	CRasterImage*		True						(const u32 size = DEFAULT_IMAGE_SIZE);

			static	CRasterImage*		BitFalse					(bool a = false);
			static	CRasterImage*		BitTrue						();

			static	CRasterImage*		Join						(CRasterImage* bitmap_x, CRasterImage* bitmap_y);
			static	void				Split						(CRasterImage* img, CRasterImage* result1, CRasterImage* result2);
			
			static	CRasterImage*		Repeat						(CRasterImage* img, int a = 1, bool destroy = true);

			static	CRasterImage*		Clone						(CRasterImage* img);

					CRasterImage*		Invert						();

			static	CRasterImage*		CreateText					(LPCSTR text, u32 x, u32 y, u32 w, u32 h, u32 s = 36, SColor background = WHITE);

					void				Load						(HBITMAP hObject);

					void				Destroy						();
			        CRasterImage*		Expand						(u32 expansion);
					CRasterImage*		ExpandGrid					(u32 expansion, SColor c = GREY);
					
					CRasterImage*		SubImage					(u32 x, u32 y, u32 w, u32 h);
				

	private:
		CxImage		m_data;
		EImageType	m_format;
};

#endif