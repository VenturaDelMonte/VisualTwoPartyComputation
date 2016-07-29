/*
	file name: BaseImage.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 15/04/13
*/

#pragma once
#ifndef __BASE_IMAGE_H__
#define __BASE_IMAGE_H__

#include "Color.h"

template <typename ElementType>
class ABSTRACT IPureBaseImage 
{
	public:
		typedef		ElementType			ELEMENT_TYPE;
		virtual							~IPureBaseImage		()																{}
		virtual		ElementType			Get					(u32 i, u32 j)													= 0;
		virtual		void				Set					(u32 i, u32 j, ElementType val)									= 0;
		virtual		u32					getWidth            () const														= 0;
		virtual		u32					getHeight           () const														= 0;		
		virtual		void				setRegion			(u32 x, u32 y, u32 w, u32 h, ELEMENT_TYPE* pixels, u32 dim)		= 0;
		virtual		void				setRegion			(u32 x, u32 y, u32 w, u32 h, int* pixels, u32 dim)				= 0;
	
		
};

namespace detail
{
	template <typename T = NullType> struct SColorHelper
	{
		static SColor ConvertTo(T b)
		{
			STATIC_CHECK(0, This_shouldnt_be_compiled);
		}
	};

	template <> struct SColorHelper <SColor>
	{
		static SColor ConvertTo(SColor b)
		{
			return b;
		}
	};

	template <> struct SColorHelper <int>
	{
		static SColor ConvertTo(int b)
		{
			return SColor(b);
		}
	};

	template <typename T, typename O = T, class C = IPureBaseImage<T>>
	struct SRegionFiller
	{
		static void fillRegion(C* img, u32 x, u32 y, u32 w, u32 h, O* pixels, u32 dim)
		{
			u32 off;
			for (u32 yy = y, yoff = 0; yy < y + h; yy++, yoff += dim) 
			{
				off = yoff;
				for (u32 xx = x; xx < x + w; xx++, off++)
				{
					T px = detail::SColorHelper<T>::ConvertTo(pixels[off]);
					img->Set(xx, yy, px);
				}
			}
		}
	};
}

#endif
