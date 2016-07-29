/*
	file name: VCSDecrypter.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 16/05/2013
*/
#pragma once
#ifndef __VCSDECRYPTER_H__
#define __VCSDECRYPTER_H__

#include "PureDecrypter.h"

template 
<
	class ImageType = CBitmap,
	u16 MAX_INPUT	= 2
>
class CVCSDecrypter : public IPureDecrypter<ImageType>
{
	public:
		typedef IPureDecrypter<ImageType>		Base;
		typedef typename Base::CImageType		CImageType;
		typedef typename Base::IMAGE_STORAGE	IMAGE_STORAGE;
		
	public:
		inline								CVCSDecrypter				();
		virtual								~CVCSDecrypter				();

		virtual		CImageType*				Decrypt						(IMAGE_STORAGE& st);
		virtual		CImageType*				Decrypt						();
		virtual		IPureDecrypter&			AddShare					(CImageType* img)				{ m_input.push_back(img); return *this; }


	protected:
		IMAGE_STORAGE				m_input;
	
};

#include "VCSDecrypter_impl.h"




#endif
