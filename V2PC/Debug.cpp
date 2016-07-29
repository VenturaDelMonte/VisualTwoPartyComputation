#include "stdafx.h"
#include "Debug.h"
#include "MainForm.h"
#include "V2PC.h"

SFileLogger* Logger = NULL;

// read here http://www.codeproject.com/Articles/9903/Calling-Managed-Code-from-Unmanaged-Code-and-vice
namespace detail
{

	 LogCallback log_callback;

#	pragma unmanaged

	void __cdecl Log_impl(const char *fmt, ...)
	{ 
		char a[1024];
		va_list va;
		va_start(va, fmt);
		u32 n = _vsnprintf(a, sizeof(a) - 1, fmt, va);
		a[n] = 0;
		va_end(va);
		// inserire qui chiamata alla LogBox
		if (log_callback)
			log_callback(a);
#	ifdef DEBUG
		OutputDebugString(a);
		OutputDebugString("\n");
#	endif
		Logger->Append(a);
	}

	void __cdecl Error_impl(const char *fmt, ...) THROWS_EXCEPTION
	{
		char a[1024];
		va_list va;
		va_start(va, fmt);
		u32 n = _vsnprintf(a, sizeof(a) - 1, fmt, va);
		a[n] = 0;
		va_end(va);
		// inserire qui chiamata alla LogBox
		if (log_callback)
			log_callback(a);
#	ifdef DEBUG
		OutputDebugString(a);
		OutputDebugString("\n");
#	endif
		Logger->Append(a);
		THROW_ERROR(a);
	}
#	pragma managed
}

void InitLogger()
{
	using namespace System;
    using namespace System::Runtime::InteropServices;

	Logger = New<SFileLogger>();
	detail::log_callback = (detail::LogCallback) Marshal::GetFunctionPointerForDelegate(V2PC::GlobalData::MainFormIstance->log_callback).ToPointer();

}