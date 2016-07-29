/*
	file name: MVCSDecrypter_impl.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 16/05/2013
*/
#pragma once

#define TEMPLATE_DEF		template <class ImageType, u16 MAX_INPUT>
#define CSMVCSDecrypter		CMVCSDecrypter<ImageType, MAX_INPUT>


TEMPLATE_DEF
CSMVCSDecrypter::CMVCSDecrypter() : CVCSDecrypter<ImageType, 2>()
{
}

TEMPLATE_DEF
CSMVCSDecrypter::~CMVCSDecrypter()
{
}

TEMPLATE_DEF 
void CSMVCSDecrypter::DecryptEx(IMAGE_STORAGE &res)
{
	IMAGE_STORAGE s12, s13;
	s12.push_back(m_input[0]);  s12.push_back(m_input[1]);
	s13.push_back(m_input[0]);  s13.push_back(m_input[2]);
	//s23.push_back(m_input[1]);  s23.push_back(m_input[2]);

	CImageType* i0 = __super::Decrypt(s12);
	CImageType* i1 = __super::Decrypt(s13);
	//CImageType* i2 = __super::Decrypt(s23);

	res.push_back(i0);
	res.push_back(i1);
	//ret.push_back(i2);

}

#undef TEMPLATE_DEF
#undef CSMVCSDecrypter
