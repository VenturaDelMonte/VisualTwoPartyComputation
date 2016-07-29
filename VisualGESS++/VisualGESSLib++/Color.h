/*
	file name: Color.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 15/04/13
*/
#pragma once
#ifndef __COLOR_H__
#define __COLOR_H__

#define ALPHA_CHANNEL_SUPPORT

#ifdef ALPHA_CHANNEL_SUPPORT
#define EXTRACT_RGBQUAD(rgb) rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue, rgb.rgbReserved
#else
#define EXTRACT_RGBQUAD(rgb) rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue
#endif

struct SColor
{
	public:
		u8 r, g, b;
#ifdef ALPHA_CHANNEL_SUPPORT
		u8 a;
#endif
	public:
		inline	SColor	(u32 c)									{  Set(c);				}
		inline	SColor	() 										{  Set(0xffffffff); }
		inline	SColor	(u8 _r, u8 _g, u8 _b
#ifdef ALPHA_CHANNEL_SUPPORT
			, u8 _a = 255)	
#else
			)
#endif
		{
			Set(_r, _g, _b
#ifdef ALPHA_CHANNEL_SUPPORT
				, _a); 
#else
				);
#endif
		}
		inline	SColor	(RGBQUAD rgb)							{  Set(EXTRACT_RGBQUAD(rgb)); }
		virtual	~SColor	()										{  }

		inline u32	Get	() const														
		{
			return RGB(r, g, b) 
#ifdef ALPHA_CHANNEL_SUPPORT
				| ((a & 0xff) << 24);
#else
			;
#endif
		}
		inline SColor& Set (u8 _r, u8 _g, u8 _b
#ifdef ALPHA_CHANNEL_SUPPORT
			, u8 _a)		
#else
			)
#endif
		{ 
			r = _r; 
			g = _g; 
			b = _b; 
#ifdef ALPHA_CHANNEL_SUPPORT
			a = _a;
#endif
			return *this;
		}
		inline SColor& Set (u32 c) // argb
		{
			r = GetRValue(c);	
			g = GetGValue(c);
			b = GetBValue(c);
#ifdef ALPHA_CHANNEL_SUPPORT
			a = LOBYTE(c >> 24);
#endif
			return *this;
		}

		inline SColor& operator= (const SColor& s)
		{
#ifdef ALPHA_CHANNEL_SUPPORT
			a = s.a;
#endif
			r = s.r;
			g = s.g;
			b = s.b;
			return *this;
		}

		inline SColor& operator= (const RGBQUAD& s)
		{
			Set(EXTRACT_RGBQUAD(s));
			return *this;
		}

		inline operator const u32 ()
		{
			return Get();
		}

		inline operator const RGBQUAD ()
		{
			RGBQUAD ret;
			ret.rgbBlue = b;
			ret.rgbGreen = g;
			ret.rgbRed = r;
#ifdef ALPHA_CHANNEL_SUPPORT
			ret.rgbReserved = a;
#endif
			return ret;
		}

		inline SColor& operator += (const SColor& c1)
		{
#ifdef ALPHA_CHANNEL_SUPPORT
			s32 a = (this->a * c1.a) / 255;
#endif
			s32 b = (this->b * c1.b) / 255;
			s32 g = (this->g * c1.g) / 255;
			s32 r = (this->r * c1.r) / 255;
			Set(r, g, b
#ifdef ALPHA_CHANNEL_SUPPORT
				, a);
#else
				);
#endif
			return *this;
		}

		inline SColor Invert()
		{
			return SColor(255 - r, 255 - g, 255 - b
#ifdef ALPHA_CHANNEL_SUPPORT
				, 255);
#else
				);
#endif
		}
};

inline	bool operator== (const SColor& c1, const SColor& c2)
{
	return c1.Get() == c2.Get();
}

inline	bool operator!= (const SColor& c1, const SColor& c2)
{
	return !(c1 == c2);
}

inline SColor operator+ (const SColor& c1, const SColor& c2)
{
#ifdef ALPHA_CHANNEL_SUPPORT
//	s32 a = ((c2.a * c1.a) / 255);
	s32 a = (c2.a > c1.a) ? c2.a : c1.a;
#endif
	s32 b = (c2.b * c1.b) / 255;
	s32 g = (c2.g * c1.g) / 255;
	s32 r = (c2.r * c1.r) / 255;
	return SColor(r, g, b
#ifdef ALPHA_CHANNEL_SUPPORT		
		, a);
#else
		);
#endif
}

#define WHITE	SColor(255,		255,	255)
#define WHITEA	SColor(255,		255,	255,	0)
#define BLACK	SColor(0,		0,		0,		255)
#define RED		SColor(255,		0,		0)
#define GREEN	SColor(0,		255,	0)
#define BLUE	SColor(0,		0,		255)
#define GREY	SColor(128,		128,	128)


#endif