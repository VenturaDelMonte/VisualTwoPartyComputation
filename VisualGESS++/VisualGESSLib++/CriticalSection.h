/*
	file name: Thread.h
	author: Ventura Del Monte
	created on: 10/07/13
*/
#pragma once
#ifndef __CRITICALSECTION_H__
#define __CRITICALSECTION_H__

class VGESS_API CCriticalSection
{
	public:
								CCriticalSection				();
		virtual					~CCriticalSection				();

						void	Enter							();
						void	Leave							();
						int		TryEnter						();
	private:
									CCriticalSection				(CCriticalSection const &)		{ }


		CRITICAL_SECTION*		m_mutex;
};

class CCriticalSectionEx
{
	public:
								CCriticalSectionEx				(CCriticalSection* cs)			 {	m_cs = cs; cs->Enter(); }
		virtual					~CCriticalSectionEx				()								 {	m_cs->Leave();			}			
	private:
		CCriticalSection* m_cs;
};

#endif