// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifdef _DEBUG
#define DEBUG
#endif

#define _CRT_SECURE_NO_WARNINGS

#pragma warning (disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include <Windows.h>
#include <winnt.h>
#include <math.h>


#ifdef BUILD_VGESS
#define VGESS_API __declspec(dllexport)
#else
#define VGESS_API __declspec(dllimport)
#endif

#define ABSTRACT __declspec(novtable)
#define NOINLINE __declspec(noinline)

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Alert Msg: "

#define alert(msg) message(__LOC__ #msg "\n")

#include "types.h"
#include "StdExtension.h"
#include "Debug.h"
#include "Memory.h"
#include "file_system.h"
#include "Reader.h"
#include "Writer.h"
#include "fastdelegate/FastDelegate.h"
#include "Random.h"
#include "FileLogger.h"
#include "ThreadPool.h"

enum ESerializationMode
{
	eAlways,
	eOnlyInputs,
	eNever,
};

// TODO: reference additional headers your program requires here
