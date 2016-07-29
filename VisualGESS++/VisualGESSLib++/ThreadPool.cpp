/*
	file name: ThreadPool.h
	author: Ventura Del Monte
	created on: 14/05/13
*/

#include "stdafx.h"
#include "ThreadPool.h"
#include <process.h>

static	u32			thread_id_generator			= 0;
static	const u32	stack_size					= 1024 * 1024;
static	const u32   default_sleep_time		    = 200;

namespace detail
{
	void __stdcall DummyProgressCallback(float progress)
	{
		//Log("progress = %f", progress);
	}
};

ProgressCallback progress_callback				= detail::DummyProgressCallback;

CThread::CThread(volatile CThreadPool* owner)	:
	m_id(thread_id_generator++), m_progress(0.f), m_done(false), m_started(false), m_owner(owner), m_thread_handle(INVALID_HANDLE_VALUE)
{
	if (m_owner)
		m_owner->AddWorker(this);
}

void CThread::Start()
{
	if (m_started) 
		return;
	m_thread_handle = (HANDLE) _beginthread(CThread::ThreadStartup, stack_size, (void*) this);
	m_started = true;
//	Log("Thread #%d starts now", m_id);
}

CThreadPool::CThreadPool()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	m_processors_count = info.dwNumberOfProcessors;
	SetThreadIdealProcessor(GetCurrentThread(), 0);
}

void CThreadPool::AddWorker(CThread* thread) volatile
{
//	if (!thread->m_started) thread->Start();
	CThreadPool* self = const_cast<CThreadPool*>(this);
	int ideal_processor = thread->m_id % m_processors_count;
	self->m_pool.push_back(thread);
	SetThreadIdealProcessor(thread->m_thread_handle, clamp(ideal_processor, 1, s32(m_processors_count - 1)));
}

void CThreadPool::WaitWorkers()
{
	const u32 pool_size = m_pool.size();
	while (pool_size)
	{
		Sleep(default_sleep_time);
		
		u32 completed = 0;
		float progress = 0;
		for (u32 i = 0; i < pool_size; i++)
		{
			if (!m_pool[i]->m_started)
				m_pool[i]->Start();
			progress += m_pool[i]->m_progress;
			completed += (m_pool[i]->m_done ? 1 : 0);
		}
	//	Log("progress = %d completed = %d", progress / pool_size, completed);
		progress_callback(progress / float(pool_size));
		if (completed == pool_size)
			break;
	}

}

void CThread::ThreadStartup(void* param)
{
	CThread* thread = (CThread*) param; // ASSERT(thread, "invalid thread");
	thread->Run();
	thread->m_done = true;
}

void SetProgressCallback(ProgressCallback callback)
{
	progress_callback = callback;
}

CThreadPool::~CThreadPool()
{
	m_pool.clear();
}