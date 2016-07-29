/*
	file name: Writer.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 27/03/2013
*/

#pragma once
#ifndef __WRITER_H__
#define __WRITER_H__

#pragma warning(disable:4267)

class VGESS_API CWriter
{
	private:
		FILE*				m_fp;
		DWORD				m_pos, m_size;
	public:
											CWriter				(FILE* fp) : m_fp(fp), m_size(0), m_pos(0) { Load(); }

		virtual								~CWriter			();
		
		template <typename T> inline void	Write				(T value)
		{
			Write										(&value, sizeof(T));
		}
	
		template <typename T> inline void	Write				(T* value)
		{
			Write										(value, sizeof(T));
		}

		template <>			  inline void	Write<char*>		(char *value)
		{
			Write										(value, strlen(value));
		}
		

	public:
		void							Write				(const void *ptr, u32 size)
		{
			u32 written					= fwrite(ptr, sizeof(char), size, m_fp);
			m_pos						+= written;
		}


	/*	
		IC void	WriteString				(char* value)
		{
			DWORD			written		= 0;
			const void*		ptr			= (const void*) value;
			const size_t	size		= strlen(value);
			WriteFile									(m_handle, ptr, size, &written, 0);
			m_pos						+= written;
		}
	*/
		void							Seek				(u32 ofs);
		u32								Size				() const		{ return m_size; }
		u32								Tell				() const		{ return m_pos; }
		void							Skip				(u32 ofs);
		void							Close				();
		FILE*							Handle				()				{ return m_fp; }
	
	protected:
		void							Load				();
};

#pragma warning(default:4267)


#endif 