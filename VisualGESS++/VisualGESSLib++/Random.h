/*
	file name: Random.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 27/03/2013
*/

#pragma once
#ifndef __RANDOM_H__
#define __RANDOM_H__

class CRandom
{
	// formula presa da qui: http://stackoverflow.com/questions/6793065/understanding-the-algorithm-of-visual-cs-rand-function
	public:
		inline			CRandom				(s32 seed = 1) : m_seed(seed)	{  }
	
		inline	s32		Rand				()								{ return (((m_seed = m_seed * 214013L + 2531011L) >> 16) & 0x7fff); }
		inline	s32		Rand				(s32 max)						{ ASSERT(max, "invalid max"); return Rand() % max; } 
		inline	s32		Rand				(s32 min, s32 max)				{ return min + Rand(max - min); }
		inline	float	RandF				()								{ return float(Rand()) / 32767.f; }
		inline 	float	RandF				(float max)						{ return RandF() * max; }
		inline 	float	RandF				(float min, float max)			{ return min + RandF(max - min); }

		inline	void	SetSeed				(s32 seed = 1)					{ m_seed = seed; }
		inline	s32		GetSeed				() const						{ return m_seed; }

	private:
		volatile s32 m_seed;

};

inline u64 GetQueryPerformaceCounter()
{
	u64	QPC;
	QueryPerformanceCounter((PLARGE_INTEGER) &QPC);
	return QPC;
}

extern VGESS_API CRandom Random;

namespace detail 
{
	struct SContainerShuffler
	{
		struct SShuffleHelper
		{
			int operator() (CRandom& r, int x)	{ return r.Rand(x); }
		};
		template <typename T, int N> static void Shuffle(CRandom& r, T (&array)[N])
		{
			SShuffleHelper			s;
			for (int i = N - 1; i > 0; --i)
			{
				std::swap(array[i], array[s(r, i + 1)]);
			}
		}
		template <typename T> static void Shuffle(CRandom& r, T array, u32 N)
		{
			SShuffleHelper			s;
			for (int i = N - 1; i > 0; --i)
			{
				std::swap(array[i], array[s(r, i + 1)]);
			}
		}
		template <typename T> static void Shuffle(CRandom& r, CVector<T>& c)
		{
			SShuffleHelper			s;
			CVector<T>::iterator	first	= c.begin();
			CVector<T>::iterator	last	= c.end();
			int						n		= c.size();
			for (int i = n - 1; i > 0; --i) 
			{
				std::swap(first + i, first + s(r, i + 1));
			}
		}
		template <typename K, typename T> static void Shuffle(CRandom& r, CAssocVector<K, T>& c)
		{
			SShuffleHelper					s;
			CAssocVector<K, T>::iterator	first	= c.begin();
			CAssocVector<K, T>::iterator	last	= c.end();
			int								n		= int(last - first);
			for (int i = n - 1; i > 0; --i) 
			{
				Loki::swap(first + i, first + s(r, i + 1));
			}
		}
		
	};
};

template <typename T> inline void Shuffle(T& c)
{
	Shuffle(::Random, c);
}

template <typename T> inline void Shuffle(CRandom& r, T& c)
{
	detail::SContainerShuffler::Shuffle(r, c);
}

template <typename T> inline void Shuffle(CRandom& r, T* c, u32 count)
{
	detail::SContainerShuffler::Shuffle(r, c, count);
}

#endif