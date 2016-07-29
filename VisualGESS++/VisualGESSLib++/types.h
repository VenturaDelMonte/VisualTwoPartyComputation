/*
	file name: types.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 27/03/2013
*/
#pragma once

typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed __int64 s64;
typedef unsigned __int64 u64;

typedef int BOOL;

typedef	char STRING_PATH[2 * _MAX_PATH];

namespace detail
{
	template <class T> struct is_class				
	{
		struct _is_class	
		{	
			char v[1];	
		};

		struct _is_not_class	
		{	
			char v[2];	
		};

		template <class O> static _is_class _checker(void(O::*)(void));
		template <class O> static _is_not_class	_checker(...);

		enum
		{	
			result = (sizeof(_is_class) == sizeof(_checker<T>(0)))	
		};
	};
}

typedef void* (__cdecl *MallocFunction)(size_t);
typedef void (__cdecl *FreeFunction)(void*);
typedef void* (__cdecl *ReallocFunction)(void*,size_t);
