#include "stdafx.h"
#include "InputPanel.h"

void V2PC::InputPanel::setLabel(int i)
{
	this->label1->Text = "x " + i + ":";
	this->radioButton1->Checked = true;
}