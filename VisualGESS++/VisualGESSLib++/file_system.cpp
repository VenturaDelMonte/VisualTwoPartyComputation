/*
	file name: file_system.cpp
	author: Ventura Del Monte & Anna De Angelis
	created on: 14/05/13

*/

#include "stdafx.h"
#include <io.h>     
#include <sys/types.h>  
#include <sys/stat.h> 
#include <direct.h>

CFileSystem FS;

CFileSystem::CFileSystem()
{

}

CFileSystem::~CFileSystem()
{

}

CReader* CFileSystem::r_open(LPCSTR file_name)
{
	FILE* fp = fopen(file_name, "rb");
	ASSERT(fp, "can't open file");
	return New<CReader>(fp, file_name);
}

CWriter* CFileSystem::w_open(LPCSTR file_name)
{
	FILE* fp = 0;
	check_dir(file_name);
	fp = fopen(file_name, "wb");
	ASSERT(fp, "can't open file");
	return New<CWriter>(fp);
}

bool DirectoryExists(LPCSTR path)
{
	struct stat status;
	if (_access(path, 0 ) == 0)
	{
		stat(path, &status);
		return (status.st_mode & S_IFDIR) != 0;
	}
	return false;
}

void CFileSystem::check_dir(LPCSTR file_name)
{
	string str = file_name;
	size_t found = str.find_last_of("/\\");
	string path = str.substr(0, found);
	if (!DirectoryExists(*path))
		mkdir(*path);
}