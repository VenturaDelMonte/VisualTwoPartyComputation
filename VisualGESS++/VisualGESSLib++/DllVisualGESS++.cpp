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

/*
	file name: DIIVisualGess++.cpp
	author: Ventura Del Monte & Anna De Angelis
	created on: 1/07/13
*/
#include "stdafx.h"
#include "Circuit.h"
#include "Parser.h"
#include "DescentParser.h"
#include "RasterImage.h"
#include "CircuitIO.h"
#include "VisualCrypt.h"
#include "MVCSEncrypter.h"
#include "color_list.h"
#include "VCSEncrypter.h"
#include "MVCSDecrypter.h"

#include "VCSDecrypter.h"
#include <time.h>
#include "ximadef.h"
/*
extern "C"
{
	extern void __cdecl _initp_misc_cfltcvt_tab();
	extern void __cdecl _fpmath(int);
}
*/

CRandom Random;
string g_output_folder = "c:";

u32 g_start_time = 1;

extern "C" DLL_EXP void CxSetMemoryManager(MallocFunction new_malloc, FreeFunction new_free, ReallocFunction new_realloc);

extern "C" VGESS_API void ExecuteV2PC(LPCSTR path, LPCSTR f, CAssocVector<u32, bool>& v, u32 image_size = 8, ESerializationMode mode = eNever, u32 expansion = 0)
{

	CCircuit* circuit = New<CCircuit>(image_size, mode);
	CDescentParser parser(*circuit);

	g_output_folder.assign(path);

	bool ret = parser.Parse(f);

	ASSERT(v.size() == parser.getNumberVariables(), "variables dont match");
	ASSERT(ret, "Parsing Failed");
	circuit->SetExpansion(expansion);
	circuit->BuildShares();
	circuit->Solve(v);
	Delete(circuit);
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ulCallReason, LPVOID lpReserved)
{

	switch (ulCallReason) 
	{
		case DLL_PROCESS_ATTACH: 
		{

			g_start_time = (u32) time(NULL); 		
			::Random.SetSeed(g_start_time);
			srand(g_start_time);
			CxSetMemoryManager(v2pc::malloc, v2pc::free, v2pc::realloc);
			break;
		}

		case DLL_PROCESS_DETACH: 
		{
			break;
		}

		case DLL_THREAD_ATTACH:
		{
			CoInitializeEx(NULL, COINIT_MULTITHREADED);
			break;
		}

		case DLL_THREAD_DETACH:
		{
			break;
		}

	}
    return TRUE;
}

extern "C" VGESS_API CircuitIO ValidateFunction(LPCSTR f, u32 image_size)
{
	CircuitIO io = {0,0};
	CCircuit* c = New<CCircuit>(image_size);
	CDescentParser parser(*c);
	if (parser.Parse(f))
	{
		io.circuit = c;
		io.inputs = c->GetInputsCount();
		return io;
	}
	return io;
}

extern "C" VGESS_API void BuildShares(CCircuit* ptr)
{
	ASSERT(ptr, "wrong pointer");
	ptr->BuildShares();
}

extern "C" VGESS_API CRasterImage* SolveCircuit(CCircuit* ptr, CAssocVector<u32, bool> in, LPCSTR path)
{
	g_output_folder.assign(path);
	ASSERT(ptr, "wrong pointer");
	return ptr->Solve(in);
}