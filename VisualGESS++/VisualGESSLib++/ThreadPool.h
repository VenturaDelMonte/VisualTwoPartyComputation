/*
	file name: Thread.h
	author: Ventura Del Monte
	created on: 10/07/13
*/
#pragma once
#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

typedef void (__stdcall *ProgressCallback)(float progress);
extern "C" VGESS_API void SetProgressCallback(ProgressCallback callback);

class CThreadPool;

class ABSTRACT CThread
{
	friend class CThreadPool;
	public:
									CThread					(volatile CThreadPool* owner);
		virtual						~CThread				()									{ }

		virtual		void			Start					(); 
		virtual		void			Run						()									= 0;

	protected:	
		volatile	u32					m_id;
		volatile	float				m_progress;
		volatile	bool				m_done, m_started;
		volatile	CThreadPool*		m_owner;
					HANDLE				m_thread_handle;

	private:
		static void					ThreadStartup			(void* param);
};


class CThreadPool
{
	public:
								CThreadPool					();
		virtual					~CThreadPool				();								
				void			AddWorker					(CThread* thread) volatile;
				void			WaitWorkers					();

	private:
		CVector<CThread*>		m_pool;
		u32						m_processors_count;
};

#endif