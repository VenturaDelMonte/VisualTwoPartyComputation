/*
	file name: StdExtension.h
	author: Ventura Del Monte
	created on: 27/03/2013
*/

#pragma once
#ifndef __STDEXTENSION_H__
#define __STDEXTENSION_H__

namespace detail
{
	template <typename T> struct STypeHelper
	{
		typedef T type;
		typedef T* ptype;
	};

	template <typename T> struct STypeHelper <T*>
	{
		typedef T type;
		typedef T* ptype;
	};

	template <typename T> struct STypeHelper <const T*>
	{
		typedef T type;
		typedef T* ptype;
	};
}

#include "Memory.h"

template <typename T> class	CAllocator	
{
	public:
		typedef ptrdiff_t	difference_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef T			value_type;
		typedef	size_t		size_type;

	public:
		template<typename O> struct rebind			
		{
			typedef CAllocator<O> other;
		};
	public:
								pointer					address			(reference value) const					{	return (&value);	}
								const_pointer			address			(const_reference _Val) const			{	return (&value);	}
														CAllocator		()										{	}
														CAllocator		(const CAllocator<T>&)					{	}
		template <typename O>							CAllocator		(const CAllocator<O>&)					{	}
		template <typename O>	CAllocator<T>&			operator=		(const CAllocator<O>&)					{	return (*this);	}
		
								pointer					allocate		(size_t n, const void* p = 0) const		{	return MemHandler.Alloc<T>(u32(n));	}
								char*					_charalloc		(size_t n)								{	return (char*) allocate(n); }
								void					deallocate		(pointer p, size_t n) const				{	MemHandler.Free(p);				}
								void					deallocate		(void* p, size_t n) const				{	MemHandler.Free(p);				}
								void					construct		(pointer p, const T& value)				{	std::_Construct(p, value);	}
								void					destroy			(pointer p)								{	std::_Destroy(p);			}
								size_t					max_size		() const								{	size_t cnt = (size_t)(-1) / sizeof (T);	return (0 < cnt ? cnt : 1);	}
};

template <typename T, typename O> inline bool operator==		(const CAllocator<T>&, const CAllocator<O>&) { return sizeof(O) == sizeof(T) && typeid(O) == typeid(T); }
template <typename T, typename O> inline bool operator!=		(const CAllocator<T>&, const CAllocator<O>&) { return !(this == c); }


#include <string.h>
#include <string>
#include <vector>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <iterator>

typedef std::basic_string<char, std::char_traits<char>, CAllocator<char>> string;

inline string __cdecl format(const char *fmt, ...)
{
	char a [1024];
	va_list va;
	va_start(va, fmt);
	_vsnprintf(a, sizeof(a) - 1, fmt, va);
	a[sizeof(a) - 1] = 0;
	va_end(va);
	return string(a);
}

inline LPCSTR operator* (string& str)
{
	return str.c_str();
}

inline LPCSTR operator* (const string& str)
{
	return str.c_str();
}

/*
IC const char* operator* (string*& str)
{
	return str->c_str();
}
*/


template <typename T, typename allocator = CAllocator<T> > class VGESS_API CVector : public std::vector<T, allocator> 
{
	private:
		typedef std::vector<T, allocator> base;

	public:
		typedef allocator allocator_type;

	public:
								CVector			()									: base()					{	}
								CVector			(size_t _count, const T& _value)	: base(_count,_value)		{	}
		explicit				CVector			(size_t _count)						: base(_count)				{	}
		virtual					~CVector		()									{ clear(); }
					u32			size			() const							{ return (u32)base::size();		} 
//					const_reference operator[]	(u32 _Pos) const					{ ASSERT(_Pos<size(),"wrong pos"); return (*(begin() + _Pos)); }
//					reference operator[]		(u32 _Pos)							{ ASSERT(_Pos<size(),"wrong pos"); return (*(begin() + _Pos)); }

};
/*
template <> class CVector<bool, CAllocator<bool>> : public std::vector<bool, CAllocator<bool>> 
{
	private:
		typedef std::vector<bool, CAllocator<bool>> base;

	public: 
		u32		size				() const							{ return (u32)base::size();		} 
		void	clear				()									{ erase(begin(),end());			} 
};
*/
template <typename T, typename C = std::vector<T, CAllocator<T>>> class CStack 
{
	public:
		typedef typename C::allocator_type						allocator_type;
		typedef typename allocator_type::value_type				value_type;
		typedef typename allocator_type::size_type				size_type;

		allocator_type		get_allocator	() const							{return (c.get_allocator());	}
		bool				empty			() const							{return (c.empty());			}
		u32					size			() const							{return u32(c.size());			} 
		value_type&			top				()									{return (c.back());				}
		const value_type&	top				() const							{return (c.back());				}
		void				push			(const value_type& V)				{c.push_back(V);				}
		void				pop				()									{c.pop_back();					}
		bool				operator==		(const CStack<T, C>& V) const		{return (c == V.c);				}
		bool				operator!=		(const CStack<T, C>& V) const		{return (!(*this == V));		}
		bool				operator<		(const CStack<T, C>& V) const		{return (c < V.c);				}	
		bool				operator>		(const CStack<T, C>& V) const		{return (V < *this);			}
		bool				operator<=		(const CStack<T, C>& V) const		{return (!(V < *this));			}
		bool				operator>=		(const CStack<T, C>& V) const		{return (!(*this < V));			}
		void				clear			()									{c.clear();						}

	protected:
		C c;
};

#define DEFINE_VECTOR(name, type) typedef CVector<type> name; typedef typename CVector<type>::iterator name##_IT; typedef typename CVector<type>::const_iterator name##_CIT; 


#include "AssocVector.h"

template
<
	class K,
    class V,
    class C = std::less<K>,
    class A = CAllocator<std::pair<K, V>>
>
class CAssocVector : public Loki::AssocVector<K, V, C, A>
{
	typedef		AssocVector<K, V, C, A>						base;
public:
	u32					size			() const							{ return u32(base::size());	} 
};

#define DEFINE_ASSOC_VECTOR(name, key, type) typedef CAssocVector<key, type> name; typedef name::iterator name##_IT; typedef name::const_iterator name##_cit; 

template <class T> inline void DeleteVector(CVector<T*>& c)
{
	while (c.size())
	{
		CVector<T*>::iterator it = c.end() - 1;
		Delete(*it);
		c.erase(it);
	}
	c.clear();
}

template <class K, class V> inline void DeleteVector(CAssocVector<K, V*>& c)
{
	while (c.size())
	{
		CAssocVector<K, V*>::iterator it = c.end() - 1;
		Delete((*it).second);
		c.erase(it);
	}
	c.clear();
}

class NullType
{

};

#define DEFINE_PAIR(name, type1, type2) typedef std::pair<type1, type2> name;


template <typename T> inline T clamp(T& value, T min, T max)
{
	if (value < min)
		value = min;
	if (value > max)
		value = max;
	return value;
}

#endif