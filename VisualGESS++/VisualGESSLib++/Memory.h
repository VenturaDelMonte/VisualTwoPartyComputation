/*
	file name: Memory.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 27/03/2013
*/

#pragma once
#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "types.h"
#include <new>

#include "CriticalSection.h"

class VGESS_API CMemoryHandler
{

	public:
												CMemoryHandler						();
		virtual									~CMemoryHandler						();
				
									void*		Alloc								(u32 size = 1);
									void*		Realloc								(void* ptr, u32 new_size);
									void		Free								(void*& ptr);

									u32			AllocatedSize						() const;
									u32			FreeSize							() const;

	private:
		template <typename T, 
			bool is_class>			void		DestroyObject		(T* pointer)
		{
			if (is_class)
				pointer->T::~T();
		}

	public:

		template <typename T> inline void		Free								(T*& pointer)
		{
			try
			{
				DestroyObject<T, detail::is_class<T>::result>(pointer);
				Free(reinterpret_cast<void*&>(pointer));
			}
			catch (...)
			{
			}
		}

					
		template <typename T> inline	T*		Alloc								(u32 count = 1)
		{
			return (T*) Alloc(count * sizeof(T));
		}

	private:
				void*				m_space;

				CCriticalSection	m_cs;

				u32					m_start_memory, m_end_memory;

};

extern VGESS_API CMemoryHandler MemHandler;

template <typename T> inline T* New()
{
	T* ptr = (T*) (MemHandler.Alloc(sizeof(T)));
	return new (ptr) T();
}

template <class T, class P1> inline T* New(P1& p1)
{
	T* ptr = (T*) (MemHandler.Alloc<T>());
	return new (ptr) T(p1);
}

template <class T, class P1, class P2> inline T* New(P1& p1, P2& p2)
{
	T* ptr = (T*) (MemHandler.Alloc<T>());
	return new (ptr) T(p1, p2);
}

template <class T, class P1, class P2, class P3> inline T* New(P1& p1, P2& p2, P3& p3)
{
	T* ptr = (T*) (MemHandler.Alloc<T>());
	return new (ptr) T(p1, p2, p3);
}

template <class T, class P1, class P2, class P3, class P4> inline T* New(P1& p1, P2& p2, P3& p3, P4& p4)
{
	T* ptr = (T*) (MemHandler.Alloc<T>());
	return new (ptr) T(p1, p2, p3, p4);
}

template <class T> inline void	Delete(T*& ptr)
{
	if (ptr == NULL) 
		return;
	MemHandler.Free(ptr);
}

template <class T> inline void Delete(T* const &ptr)
{
	if (ptr == NULL) 
		return;
	MemHandler.Free(ptr);
}

inline void* operator new(size_t size)
{	
	return MemHandler.Alloc(size ? u32(size) : 1);	
}

inline void operator delete(void *p)
{	
	Delete(p);	
}

inline void* operator new[] (size_t size)
{	
	return MemHandler.Alloc(size ? u32(size) : 1);	
}

inline void operator delete[] (void *p)
{	
	Delete(p);	
}

inline bool IsPointerOnStack(void* ptr)
{
    void* stack_top = 0;
    void* stack_low_count = 0;
    __asm
    {
        mov eax, FS:[4]
        mov stack_top, eax
        mov eax, FS:[8]
        mov stack_low_count, eax
    }
	// non ti dimenticare del TIB per win32
    return (ptr <= stack_top && ptr >= stack_low_count);
}

namespace v2pc
{
	void* malloc(size_t size);
	void* realloc(void* ptr, size_t size);
	void free(void* ptr);
};

#endif