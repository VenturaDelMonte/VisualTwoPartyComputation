#pragma once
#include "MainForm.h"

namespace V2PC
{

	public ref class GlobalData
	{
		public:
			static MainForm^ MainFormIstance;
			static HMODULE dll;

	};


};