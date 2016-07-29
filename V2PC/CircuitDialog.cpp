#include "stdafx.h"
#include "CircuitDialog.h"
#include "MainForm.h"
#include "GateDialog.h"
#include "Gate.h"
#include "V2PC.h"

namespace V2PC
{
	extern HWND g_window_handle;

	extern Bitmap^ RasterToBitmap(CRasterImage* original, u32 expand = 16);

	void CircuitDialog::UpdateNode(CircuitImageNode* node)
	{
		Bitmap^ clone = RasterToBitmap(node->img, 0);
		this->panel->AddImage(clone, node->x, node->y, node->gate);
		
	}

	void CircuitDialog::Setup(u32 w, u32 h, u32 sw, u32 sh)
	{
		m_circuit_width = w;
		m_circuit_height = h;
		m_screen_width = sw;
		m_screen_height = sh;
		delete this->panel;
		delete components;
		InitializeComponent();
		this->panel->AutoScroll = true;
		this->panel->NodeDelegate += gcnew OnNodeClickedDelegate(this, &CircuitDialog::OnNodeClicked);
	}

	void CircuitDialog::OnNodeClicked(void* ptr)
	{
		GateDialog^ dlg = gcnew GateDialog();
		dlg->Setup((IGate*) ptr);
		dlg->Show(this);
	}

	void CircuitDialog::PrepareShares(u32 size, u32 expansion)
	{
	
		shares->Clear();
		for (int i = 0; i < gates->Count; i++)
		{
			IGate* g = (IGate*) gates[i].ToPointer();
			u32 idx = g->GetPosition()->getIndex();
			string str = format("share_G%d-L-%d", idx, 0);
			CCircuit::CSharesHolder* pHolder = dynamic_cast<CCircuit::CSharesHolder*>(g);
			CCircuit::CSharesHolder& holder = *pHolder;
			this->shares->Add(gcnew String(*str), RasterToBitmap(CCircuit::ConcatenateShare(*str, holder(0, 0), expansion, size), 0));
			str = format("share_G%d-L-%d", idx, 1);
			this->shares->Add(gcnew String(*str), RasterToBitmap(CCircuit::ConcatenateShare(*str, holder(1, 0), expansion, size), 0));

		/*	if (holder.IsInverted())
			{
				str = format("share_G%d-R-%d", idx, 0);
				this->shares->Add(gcnew String(*str), RasterToBitmap(CCircuit::ConcatenateShare(*str, holder(1, 1), holder(0, 1), expansion, size), 0));
				str = format("share_G%d-R-%d", idx, 1);
				this->shares->Add(gcnew String(*str), RasterToBitmap(CCircuit::ConcatenateShare(*str, holder(1, 2), holder(0, 2), expansion, size), 0));
			}
			else*/
			{
				str = format("share_G%d-R-%d", idx, 0);
				this->shares->Add(gcnew String(*str), RasterToBitmap(CCircuit::ConcatenateShare(*str, holder(0, 1), holder(1, 1), expansion, size), 0));
				str = format("share_G%d-R-%d", idx, 1);
				this->shares->Add(gcnew String(*str), RasterToBitmap(CCircuit::ConcatenateShare(*str, holder(0, 2), holder(1, 2), expansion, size), 0));
			}
		}
	}
	using namespace System::Drawing::Printing;
	using namespace System::Drawing;
	void CircuitDialog::OnPrintAll(Object^ sender, EventArgs^ e)
	{
		u32 size = GlobalData::MainFormIstance->Circuit().GetShareSize();
		u32 des_expansion = Convert::ToInt32(Microsoft::VisualBasic::Interaction::InputBox(gcnew String("Shares expansion"), gcnew String("Set Shares expansion"), gcnew String("16"), 100, 100));
		PrepareShares(size, des_expansion);
		PrintDialog^ dlgPrint = gcnew PrintDialog;
		PrintDocument^ docPrint = gcnew PrintDocument;
		docPrint->DocumentName = "Input Gates";
		dlgPrint->Document = docPrint;
		docPrint->PrintPage += gcnew PrintPageEventHandler(this, &CircuitDialog::OnPrintPage);
		if (dlgPrint->ShowDialog() == Forms::DialogResult::OK)
		{
			docPrint->Print();
		}
	}
	void CircuitDialog::OnPrintPage(System::Object^ sender, PrintPageEventArgs^ e)
	{

	}
};