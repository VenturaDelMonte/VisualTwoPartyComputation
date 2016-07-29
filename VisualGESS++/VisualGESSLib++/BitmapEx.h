/*
	file name: BitMap.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 30/04/13
*/

#ifndef __BITMAPEX_H__
#define __BITMAPEX_H__

#include "Serializable.h"
#include "BaseImage.h"
#include "CImg.h"

#define DEFAULT_IMAGE_SIZE 8

class CBitmapEx;

class CBitmapEx : public ISerializable, public IPureBaseImage<SColor> 
{
	public:
								CBitmapEx				();
								CBitmapEx				(u32 width, u32 height, u16 bytes_per_pixel = 3);
			virtual				~CBitmapEx				();

			virtual	void		Save					(CWriter *fs);
			virtual	void		Load					(CReader *fs);
			
			virtual	u32			getWidth				() const												{ return m_data.width(); }
			virtual	u32			getHeight				() const												{ return m_data.height(); }		

			virtual	SColor		Get						(u32 i, u32 j);
			virtual	void		Set						(u32 i, u32 j, SColor val);
					void		Set						(u32 i, u32 j, u32 val);

					SColor*		Data					(u32 x, u32 y);

			virtual	void			setRegion			(u32 x, u32 y, u32 w, u32 h, SColor* pixels, u32 dim);
			virtual	void			setRegion			(u32 x, u32 y, u32 w, u32 h, int* pixels, u32 dim);

					void			LoadInternal		(FILE *fp);
					CBitmapEx*		Fill				(SColor c);

				   CBitmapEx*		insertImage			(CBitmapEx* img, u32 off_x, u32 off_y);

			static CBitmapEx*		False				(const u32 size = DEFAULT_IMAGE_SIZE);
			static CBitmapEx*		True				(const u32 size = DEFAULT_IMAGE_SIZE);

			static CBitmapEx*		BitFalse			(bool a = false);
			static CBitmapEx*		BitTrue				();

			static CBitmapEx*		Join				(CBitmapEx* bitmap_x, CBitmapEx* bitmap_y);
			static void				Split				(CBitmapEx* img, CBitmapEx* result1, CBitmapEx* result2);
			
			static CBitmapEx*		Repeat				(CBitmapEx* img, int a = 1, bool destroy = true);

			static CBitmapEx*		Clone				(CBitmapEx* img);

			CBitmapEx*				Invert				();

			inline bool				operator==			(const CBitmapEx& oth)		{ return oth.m_data == this->m_data; }
			
	private:
		cimg_library::CImg<u8> m_data;
};

#endif