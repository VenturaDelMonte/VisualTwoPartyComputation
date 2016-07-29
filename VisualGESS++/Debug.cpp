#include "stdafx.h"
#include "Debug.h"

SFileLogger* Logger = NULL;

void InitLogger()
{
	Logger = New<SFileLogger>();
}

namespace detail
{

void __cdecl Log_impl(const char *fmt, ...)
{
	char a[1024];
	va_list va;
	va_start(va, fmt);
	u32 n = _vsnprintf(a, sizeof(a) - 1, fmt, va);
	a[n] = 0;
	va_end(va);
	printf("* %s\n", a);
#ifdef DEBUG
	OutputDebugString(a);
	OutputDebugString("\n");
#endif
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
	printf("! %s\n", a);
#ifdef DEBUG
	OutputDebugString(a);
	OutputDebugString("\n");
#endif
	Logger->Append(a);
	THROW_ERROR(a);
}

}