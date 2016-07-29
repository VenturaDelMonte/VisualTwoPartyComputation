/*
	file name: MVCSDecrypter.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 16/05/2013
*/
#pragma once
#ifndef __MVCSDECRYPTER_H__
#define __MVCSDECRYPTER_H__


#include "PureDecrypter.h"
#include "VCSDecrypter.h"


template 
<
	class ImageType = CBitmap,
	u16 MAX_INPUT	= 3
>
class CMVCSDecrypter : public CVCSDecrypter<ImageType, 2>
{
	typedef CVCSDecrypter<ImageType, MAX_INPUT> Base;
	DEFINE_VECTOR(IMAGE_STORAGE, ImageType*);
	public:
		inline								CMVCSDecrypter				();
		virtual								~CMVCSDecrypter				();

						void				DecryptEx						(IMAGE_STORAGE &res);
	
};

#include "MVCSDecrypter_impl.h"

#endif