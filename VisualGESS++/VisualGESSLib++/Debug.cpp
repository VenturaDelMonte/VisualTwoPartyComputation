/*
	file name: Debug.cpp
	author: Ventura Del Monte
	created on: 27/03/2013
*/

#include "stdafx.h"
#include "Debug.h"

extern CFileSystem FS;



LogFunction Log = NULL;
ErrorFunction Error = NULL;

/*

*/

void InitDebug(LogFunction log_func, ErrorFunction error_func)
{
	Log = log_func;
	Error = error_func;
	
}

