/*
	file name: Reader.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 27/03/2013
*/

#pragma once
#ifndef __READER_H__
#define __READER_H__


class VGESS_API CReader 
{
	private:
			FILE*						m_fp;
			BYTE*						m_buffer;
			DWORD						m_size;
			DWORD						m_pos;
			LPCSTR						m_file_name;
	public:
											CReader				(FILE* fp, LPCSTR fn) : m_fp(fp), m_buffer(0), m_size(0), m_pos(0), m_file_name(fn) { Load(); }

		virtual								~CReader			();

		template <typename T> inline	T	Read				()
		{
			T			ret;
			u8			size = sizeof	(T);
			memcpy						(&ret, &m_buffer[m_pos], size);
			m_pos			 += size;
			return ret;
		}

		void								Seek				(u32 ofs)		{ m_pos = ofs; }
		u32									Size				() const		{ return m_size; }
		u32									Tell				() const		{ return m_pos; }
		void								Skip				(u32 ofs)		{ m_pos += ofs; }
		BYTE*								Pointer				()				{ return &m_buffer[0]; }
		FILE*								Handle				()				{ return m_fp; }
		LPCSTR								FileName			()				{ return m_file_name; }

	protected:
		void							Load				();


};


#endif