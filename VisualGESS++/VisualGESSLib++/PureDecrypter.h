/*
	file name: PureDecrypter.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 16/05/2013
*/
#pragma once
#ifndef __PURE_DECRYPTER_H__
#define __PURE_DECRYPTER_H__

template 
<
	class ImageType = CBitmap
>
class ABSTRACT IPureDecrypter
{
	public:
		typedef ImageType CImageType;
		DEFINE_VECTOR(IMAGE_STORAGE, ImageType*);
	
		virtual								~IPureDecrypter			()							{ }
		virtual		CImageType*				Decrypt					(IMAGE_STORAGE& st)			= 0;
		virtual		CImageType*				Decrypt					()							= 0;
		virtual		IPureDecrypter&			AddShare				(CImageType* img)			= 0; 
};

#endif