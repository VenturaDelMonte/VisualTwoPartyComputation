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

// V2PC.cpp : main project file.

#include "stdafx.h"
#include "V2PC.h"
#include "Debug.h"

// studia Annaaaa !! -> http://en.wikipedia.org/wiki/C%2B%2B/CLI

// ci vuole una cosa simile -> http://stackoverflow.com/questions/2196097/elegant-log-window-in-winforms-c-sharp

namespace V2PC 
{   

    using namespace System;
    using namespace System::Windows::Forms;

	void OnProcessExit(Object^ sender, EventArgs^ e)
	{
		FreeLibrary(GlobalData::dll);
	}

    [STAThread]
    int main(array<String^>^ args)
    {
		GlobalData::dll = LoadLibrary("VisualGESSLib++.dll");
		if (GlobalData::dll == INVALID_HANDLE_VALUE)
			return -1;
		AppDomain::CurrentDomain->ProcessExit += gcnew EventHandler(OnProcessExit);
        Application::EnableVisualStyles();
        Application::SetCompatibleTextRenderingDefault(false);
		GlobalData::MainFormIstance = gcnew MainForm();
		InitLogger();
		InitDebug(detail::Log_impl, detail::Error_impl);
        Application::Run(GlobalData::MainFormIstance);
        return 0;
    }
}