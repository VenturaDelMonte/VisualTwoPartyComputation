/*
	file name: VCSDecrypter_impl.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 16/05/2013
*/
#pragma once

#define TEMPLATE_DEF		template <class ImageType, u16 MAX_INPUT>
#define CSVCSDecrypter		CVCSDecrypter<ImageType, MAX_INPUT>

TEMPLATE_DEF
CSVCSDecrypter::CVCSDecrypter() 
{

}

TEMPLATE_DEF
CSVCSDecrypter::~CVCSDecrypter()
{
//	DeleteVector(m_input);
}

TEMPLATE_DEF
typename CSVCSDecrypter::CImageType* CSVCSDecrypter::Decrypt(IMAGE_STORAGE& st)
{
	m_input = st;
	return Decrypt();
}

TEMPLATE_DEF 
typename CSVCSDecrypter::CImageType* CSVCSDecrypter::Decrypt()
{
	ASSERT(m_input.size() == MAX_INPUT, "not enough or too much input shares");
	CImageType* sh1 = m_input[0]; ASSERT(sh1, "share1 not valid");
	CImageType* sh2 = m_input[1]; ASSERT(sh2, "share2 not valid");

	const u32 width = sh1->getWidth();
	const u32 height =  sh1->getHeight();
	
	CImageType* img = New<CImageType>(width, height);

	for(u32 i = 0; i < height; i++) 
	{
		for(u32 j = 0; j < width; j++)
		{
			CImageType::ELEMENT_TYPE px = sh1->Get(j, i) + sh2->Get(j, i);
			img->Set(j, i, px);
		}
	}

	return img;

}

#undef TEMPLATE_DEF
#undef CSVCSDecrypter
