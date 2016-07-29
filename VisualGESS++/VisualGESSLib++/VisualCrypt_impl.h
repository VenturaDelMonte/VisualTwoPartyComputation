/*
	file name: VisualCrypt_impl.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 14/05/13
*/

#pragma once

#define TEMPLATE_DEF template <typename EncryptionPolicy, typename DecryptionPolicy, typename ColorList>
#define CSVisualCrypter CVisualCrypter<EncryptionPolicy, DecryptionPolicy, ColorList>

TEMPLATE_DEF
inline CSVisualCrypter::CVisualCrypter() 
{
	m_colors	= New<CColorList>();
	m_encrypter = New<CEncrypter>(m_colors);
	m_decrypter = New<CDecrypter>();
}

TEMPLATE_DEF
CSVisualCrypter::~CVisualCrypter()
{
	Delete(m_encrypter);
	Delete(m_decrypter);
	Delete(m_colors);
}

#undef TEMPLATE_DEF
#undef CSVisualCrypter