/*
	file name: PureEncrypter.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 14/05/13
*/
#pragma once
#ifndef __PURE_ENCRYPTER_H__
#define __PURE_ENCRYPTER_H__

template 
<
	class ImageType = CBitmap
>
class ABSTRACT IPureEncrypter
{
	public:
		typedef ImageType CImageType;
		DEFINE_VECTOR(IMAGE_STORAGE, ImageType*);
	
		virtual								~IPureEncrypter			()							{ }
		virtual		void					Encrypt					(IMAGE_STORAGE& st, IMAGE_STORAGE& res)			= 0;
		virtual		void					Encrypt					(IMAGE_STORAGE& res)							= 0;
		virtual		IPureEncrypter&			AddImage				(CImageType* img)			= 0; 
};

#endif