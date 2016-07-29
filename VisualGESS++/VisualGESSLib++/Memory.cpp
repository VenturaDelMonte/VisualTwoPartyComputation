/*
	file name: Memory.cpp
	author: Ventura Del Monte & Anna De Angelis
	created on: 27/03/2013
*/
#include "stdafx.h"
#include "CriticalSection.h"
#pragma hdrstop
#define USE_PT3_PREFIX
#define MSPACES 1
#define USE_OUT_OF_MEMORY_HANDLER
#define USE_LOCKS 1
#include "ptmalloc/malloc-2.8.3.h"
#include "memory.h"

static	const int		size					= 64 * 1024 * 1024;
static	char			buffer					[size];

VGESS_API CMemoryHandler MemHandler;

static void __stdcall out_of_memory(void* const space, void const* const parameter, int const first_time)
{
	if (first_time)
		return;
	CMemoryHandler* self = (CMemoryHandler*) parameter;
	Error("not enough memory %d/%d", self->AllocatedSize(), self->FreeSize());
}

CMemoryHandler::CMemoryHandler()
{
	m_space = create_mspace_with_base(buffer, size, 0, &out_of_memory, (void*) this);
	m_start_memory = AllocatedSize();
	m_end_memory = 0;
}

CMemoryHandler::~CMemoryHandler()
{
	m_end_memory = AllocatedSize();
	destroy_mspace(m_space);
//	Log("total memory usage: %dkb\n", (m_end_memory - m_start_memory) / 1024);
}

void* CMemoryHandler::Alloc(u32 size)
{
	CCriticalSectionEx lock(&m_cs);
	void* ptr = mspace_malloc(m_space, size);
	ASSERT(ptr, "wrong ptr");
	return ptr;
}

void* CMemoryHandler::Realloc(void* ptr, u32 new_size)
{
	CCriticalSectionEx lock(&m_cs);
	void* ret = mspace_realloc(m_space, ptr, new_size);
	ASSERT(ret, "wrong ptr");
	return ret;
}

void CMemoryHandler::Free(void*& ptr)
{
	CCriticalSectionEx lock(&m_cs);
	mspace_free(m_space, ptr);
	ptr = NULL;
}

u32 CMemoryHandler::AllocatedSize() const
{
	return (u32) mspace_mallinfo(m_space).uordblks;
}

u32 CMemoryHandler::FreeSize() const
{
	return (u32) mspace_mallinfo(m_space).fordblks;
}

void* v2pc::malloc(size_t size)
{
	return MemHandler.Alloc(size);
}
void* v2pc::realloc(void* ptr, size_t size)
{
	return MemHandler.Realloc(ptr, size);
}
void v2pc::free(void* ptr)
{
	MemHandler.Free(ptr);
}