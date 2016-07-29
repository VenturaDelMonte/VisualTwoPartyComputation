 /*
	Visual two party computation protocol
    Copyright (C) 2013 - Del Monte Bonaventura, Anna De Angelis

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"
#include "Debug.h"
#include "ParserConsole.h"


extern "C" VGESS_API void ExecuteV2PC(LPCSTR path, LPCSTR f, CAssocVector<u32, bool>& v, u32 image_size = 8, ESerializationMode mode = eNever, u32 expansion = 0);

int main(int argc, char* argv[])
{
/*
	for (int i = 0; i <= 100; i++)
	{
		Sleep(100);
		printf("\rloading: %3d%%:  [", i);
		int p = i * 30 / 100;
		for (int ii = 0; ii < p; ii++)
			printf("=");
		for (int ii = 0; ii < 30 - p; ii++)
			printf(" ");
		printf("]");
	}
*/

	HMODULE dll = LoadLibrary("VisualGESSLib++.dll");
	if (dll == INVALID_HANDLE_VALUE)
	{
		Log("Unable to find library!");
		return -1;
	}

	InitLogger();
	InitDebug(detail::Log_impl, detail::Error_impl);

	CParserConsole parserConsole(argv, argc);

	ExecuteV2PC(parserConsole.output.c_str(), parserConsole.getFunction(), parserConsole.variables_vector, parserConsole.fdp, ESerializationMode::eAlways, parserConsole.exp);

	FreeLibrary(dll);
	
	
	return 0;

	

}

