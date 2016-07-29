#include "stdafx.h"
#include "CriticalSection.h"

CCriticalSection::CCriticalSection()
{
	m_mutex = (CRITICAL_SECTION*) std::malloc(sizeof(CRITICAL_SECTION));
	InitializeCriticalSection(m_mutex);
}

CCriticalSection::~CCriticalSection()
{
	DeleteCriticalSection(m_mutex);
	std::free(m_mutex);
}

void CCriticalSection::Enter()
{
	EnterCriticalSection(m_mutex);
}

void CCriticalSection::Leave()
{
	LeaveCriticalSection(m_mutex);
}

int CCriticalSection::TryEnter()
{
	return TryEnterCriticalSection(m_mutex);
}