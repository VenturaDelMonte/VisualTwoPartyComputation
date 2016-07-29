/*
	file name: Writer.cpp
	author: Ventura Del Monte & Anna De Angelis
	created on: 27/03/2013*/

#include "stdafx.h"
#include "writer.h"

CWriter::~CWriter()
{
	Close();
}

void CWriter::Close()
{
	fflush											(m_fp);
	fclose											(m_fp);
}

void CWriter::Load()
{
	fseek											(m_fp, 0, SEEK_END);
	m_size					   = ftell				(m_fp);
	fseek											(m_fp, 0, SEEK_SET);
}

void CWriter::Seek(u32 ofs)
{
	fseek											(m_fp, ofs, SEEK_SET);
	m_pos					   = ftell				(m_fp);
}

void CWriter::Skip(u32 ofs)
{
	fseek											(m_fp, m_pos + ofs, SEEK_SET);
	m_pos					   = ftell				(m_fp);
}

