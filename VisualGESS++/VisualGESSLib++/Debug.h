/*
	file name: Debug.h
	author: Ventura Del Monte
	created on: 27/03/2013
*/

#pragma once
#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifndef USE_EXCEPTION_HANDLING
#define THROWS_EXCEPTION
#define THROW_ERROR(msg) __asm int 3
#else
class InternalException : public std::exception
{
public:
	string message;
	InternalException(const char *str) 
	{
		message.assign(str);
		std::exception(*message);
	}
	InternalException(const InternalException& e)
	{
		*this = e;
	}
	virtual void operator= (const InternalException& e)
	{
		message = e.message;
	}
	virtual ~InternalException() throw() 
	{
	}
	virtual const char* what() throw()
	{
		return *message;
	}
	virtual const char* what() const throw()
	{
		return *message;
	}
};
#define THROWS_EXCEPTION throw(InternalException)
#define THROW_ERROR(msg) throw InternalException(msg)
#endif

template<int> struct CompileTimeError;
template<> struct CompileTimeError<true> { };

#define STATIC_CHECK(expr, msg) { CompileTimeError<((expr) != 0)> ERROR_##msg; (void)ERROR_##msg; } 

//#define USE_EXCEPTION_HANDLING
/*
extern "C" VGESS_API void __cdecl Log(const char *fmt, ...);
extern "C" VGESS_API void __cdecl Error(const char *fmt, ...);

void InitDebug();
*/


typedef void (__cdecl *LogFunction)(LPCSTR fmt, ...);
typedef void (__cdecl *ErrorFunction)(LPCSTR fmt, ...);

extern VGESS_API LogFunction Log;
extern VGESS_API ErrorFunction Error;

#ifdef DEBUG
#define ASSERT(expr, message) do { if (!(expr)) { Error("Assertion Failed: %s:%d %s :: %s", __FILE__, __LINE__, #expr, message); } } while (0); 
#else
#define ASSERT(expr, message) do { if (!(expr)) { Error("Assertion Failed:  %s", message); } } while (0); 
#endif

#define FATAL(message) ASSERT(0, message) 
extern "C" VGESS_API void InitDebug(LogFunction log_func, ErrorFunction error_func);

#endif