/*
	file name: VisualCrypt.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 14/05/13
*/
#pragma once
#ifndef __VISUAL_CRYPT_H__
#define __VISUAL_CRYPT_H__

class CBlackWhiteColorList;

template 
<
	typename EncryptionPolicy,
	typename DecryptionPolicy,
	typename ColorList = CBlackWhiteColorList
>
class CVisualCrypter
{
	public:
		typedef EncryptionPolicy	CEncrypter;
		typedef DecryptionPolicy	CDecrypter;
		typedef ColorList			CColorList;
	public:
		inline							CVisualCrypter				();
		virtual							~CVisualCrypter				();

		inline	CEncrypter& 			Encrypter					()			{ return *m_encrypter; }
		inline	CDecrypter&				Decrypter					()			{ return *m_decrypter; }
	private:
		CEncrypter*		m_encrypter;
		CDecrypter*		m_decrypter;
		CColorList*		m_colors;

};

#include "VisualCrypt_impl.h"

#endif
