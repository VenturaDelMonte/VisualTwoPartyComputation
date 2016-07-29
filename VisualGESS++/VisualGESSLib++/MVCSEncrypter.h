/*
	file name: MVCSEncrypter.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 14/05/13
*/
#pragma once
#ifndef __MVCSENCRYPTER_H__
#define __MVCSENCRYPTER_H__

#include "PureEncrypter.h"
#include "Color.h"
#include "VCSEncrypter.h"

class IColorList;

template 
<
	class ImageType = CBitmap,
	u16 MAX_INPUT	= 2
>
class CMVCSEncrypter : public CVCSEncrypter<ImageType, MAX_INPUT>
{
	typedef typename CVCSEncrypter<ImageType, MAX_INPUT> Base;
	public:
		inline								CMVCSEncrypter				(IColorList* colors, u16 k = 2, u16 n = 2, u16 m = 2, u16 h = 1, u16 v = 1);
		virtual								~CMVCSEncrypter				();

		virtual		void					Encrypt						(IMAGE_STORAGE& res);
};

#include "MVCSEncrypter_impl.h"

#endif
