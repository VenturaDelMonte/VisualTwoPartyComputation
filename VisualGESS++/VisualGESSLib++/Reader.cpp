/*
	file name: Reader.cpp
	author: Ventura Del Monte & Anna De Angelis
	created on: 27/03/2013
*/

#include "stdafx.h"
#include "reader.h"

CReader::~CReader()
{
	m_size = m_pos				= 0;
	Delete													(m_buffer);
	fclose													(m_fp);
}

void CReader::Load()
{
	size_t		read	       = 0;
	fseek													(m_fp, 0, SEEK_END);
	m_size					   = ftell						(m_fp);
	rewind													(m_fp);
	m_buffer				   = ::MemHandler.Alloc<BYTE>	(m_size + 1);
	memset													(m_buffer, 0, m_size);
	read					   = fread						(m_buffer, sizeof(char), m_size, m_fp);
	ASSERT													(m_size == read, "empty file?");
	m_buffer[read]			   = 0;
	rewind													(m_fp);
}


