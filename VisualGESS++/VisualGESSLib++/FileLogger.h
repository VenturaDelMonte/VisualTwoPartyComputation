#pragma once
#ifndef __FILE_LOGGER_H__
#define __FILE_LOGGER_H__

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
	
struct VGESS_API SFileLogger
{
	private:
		CWriter *m_log;
	public:
		SFileLogger					()
		{
			extern string g_output_folder;
			char exepath[MAX_PATH + 1];
			GetModuleFileName(0, exepath, MAX_PATH + 1);
			PathRemoveFileSpec(exepath);
			char path[128];
			sprintf(path, "%s\\log.log", exepath);
		//	printf(path);
			m_log = FS.w_open(path);
		}
		void Append					(char *str)
		{
			m_log->Write<char*>(str);
			m_log->Write<char*>("\r\n");
		}
};

#endif