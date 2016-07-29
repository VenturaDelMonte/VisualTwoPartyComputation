/*
	file name: file_system.h
	author: Ventura Del Monte & Anna De Angelis
	created on: 27/03/2013
*/
#pragma once
#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

class CWriter;
class CReader;


class CFileSystem  
{
	public:
							CFileSystem							();
		virtual				~CFileSystem						();
				CWriter*	w_open								(LPCSTR file_name);
				CReader*    r_open								(LPCSTR file_name);
				void		check_dir							(LPCSTR file_name);

};

extern CFileSystem FS;

#endif