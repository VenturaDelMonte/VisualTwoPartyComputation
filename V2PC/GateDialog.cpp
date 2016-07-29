#include "stdafx.h"
#include "GateDialog.h"
#include "MainForm.h"
#include "V2PC.h"
#include "Gate.h"
#include "Circuit.h"
#include "RasterImage.h"

#define DEFAULT_EXPANSION 8
#define DEFAULT_SIZE 8
namespace V2PC
{
	using namespace System::Drawing::Printing;
	using namespace System::Drawing;
	using namespace System::Threading;
	extern Bitmap^ RasterToBitmap(CRasterImage* original, u32 expand = 16);
	extern HWND g_window_handle;
	
	System::Void GateDialog::OnSave(Object^ sender, EventArgs^ e)
	{
		u32 size = GlobalData::MainFormIstance->Circuit().GetShareSize();
		u32 idx = m_gate->GetPosition()->getIndex();
		u32 des_expansion = this->textBox1->Text != "" ? Convert::ToInt32(this->textBox1->Text) : 1;
		PrepareShares(size, des_expansion);
		String^ folder = Application::StartupPath;
		m_ready_shares[0]->Save(String::Format("{0}\\share_G{1}-L-{2}.png", folder, idx, 0), Imaging::ImageFormat::Png);
		m_ready_shares[1]->Save(String::Format("{0}\\share_G{1}-L-{2}.png", folder, idx, 1), Imaging::ImageFormat::Png);
		m_ready_shares[2]->Save(String::Format("{0}\\share_G{1}-R-{2}.png", folder, idx, 0), Imaging::ImageFormat::Png);
		m_ready_shares[3]->Save(String::Format("{0}\\share_G{1}-R-{2}.png", folder, idx, 1), Imaging::ImageFormat::Png);
		MessageBox::Show(this, "Stored in " + folder);
	}

	System::Void GateDialog::OnPrint(Object^ sender, EventArgs^ e)
	{
		u32 idx = m_gate->GetPosition()->getIndex();
		u32 size = GlobalData::MainFormIstance->Circuit().GetShareSize();
		u32 des_expansion = this->textBox1->Text != "" ? Convert::ToInt32(this->textBox1->Text) : 1;
		PrepareShares(size, des_expansion);
		PrintDialog^ dlgPrint = gcnew PrintDialog;
		PrintDocument^ docPrint = gcnew PrintDocument;
		docPrint->DocumentName = String::Format("Gate{0}", idx);
		dlgPrint->Document = docPrint;
		docPrint->PrintPage += gcnew PrintPageEventHandler(this, &GateDialog::OnPrintPage);
		if (dlgPrint->ShowDialog() == Forms::DialogResult::OK)
		{
			docPrint->Print();
		}
	}

	System::Void GateDialog::OnPrintPage(System::Object^ sender, PrintPageEventArgs^ e)
	{
		u32 x = 20;
		u32 y = 20;
		for (s32 i = 0; i < m_ready_shares->Count; i++)
		{
			e->Graphics->DrawImageUnscaled(m_ready_shares[i], x, y);
	//		x += this->m_ready_shares[0]->Width + 100;


			y += this->m_ready_shares[i]->Height + 100;
			

		}
	}

	void GateDialog::AddImage(CRasterImage* im, u32 x, u32 y)
	{
		u32 w = im->getWidth();
		u32 h = im->getHeight();
		PictureBox^ box = gcnew PictureBox();
		box->Image = RasterToBitmap(im, false);
		box->Width = w;
		box->Height = h;
		box->Dock = DockStyle::Fill;
		box->Anchor = AnchorStyles::Bottom | AnchorStyles::Top;
		//box->Image->Save("c:\\aaa" + x + "." + y + ".bmp"); 
		this->panel->Controls->Add(box, x, y);
		
	}
//	extern Bitmap^ RasterToBitmap(CRasterImage* original, u32 expand = 16);
	void GateDialog::PrepareShares(u32 size, u32 expansion)
	{
		if (!m_gate)
			return;
		m_ready_shares->Clear();
		
		u32 idx = m_gate->GetPosition()->getIndex();
		string str = format("share_G%d-L-%d", idx, 0);
		CCircuit::CSharesHolder* pHolder = dynamic_cast<CCircuit::CSharesHolder*>(m_gate);
		CCircuit::CSharesHolder& holder = *pHolder;
		this->m_ready_shares->Add(RasterToBitmap(CCircuit::ConcatenateShare(*str, holder(0, 0), expansion, size), 0));
		str = format("share_G%d-L-%d", idx, 1);
		this->m_ready_shares->Add(RasterToBitmap(CCircuit::ConcatenateShare(*str, holder(1, 0), expansion, size), 0));

	/*	if (holder.IsInverted())
		{
			str = format("share_G%d-R-%d", idx, 0);
			this->m_ready_shares->Add(RasterToBitmap(CCircuit::ConcatenateShare(*str, holder(1, 1), holder(0, 1), expansion, size), 0));
			str = format("share_G%d-R-%d", idx, 1);
			this->m_ready_shares->Add(RasterToBitmap(CCircuit::ConcatenateShare(*str, holder(1, 2), holder(0, 2), expansion, size), 0));
		}
		else*/
		{
			str = format("share_G%d-R-%d", idx, 0);
			this->m_ready_shares->Add(RasterToBitmap(CCircuit::ConcatenateShare(*str, holder(0, 1), holder(1, 1), expansion, size), 0));
			str = format("share_G%d-R-%d", idx, 1);
			this->m_ready_shares->Add(RasterToBitmap(CCircuit::ConcatenateShare(*str, holder(0, 2), holder(1, 2), expansion, size), 0));
		}
	}

	void GateDialog::Setup(IGate* g)
	{
		if (g->GetType() == IGate::eInputGate)
			return;

		m_gate = g;
	
		CCircuit::CSharesHolder* pHolder = dynamic_cast<CCircuit::CSharesHolder*>(g);
		CCircuit::CSharesHolder& holder = *pHolder;

		u32 size = GlobalData::MainFormIstance->Circuit().GetShareSize();

		this->panel->Controls->Clear();
		
		this->AddImage(CCircuit::ConcatenateShare(0, holder(0, 0), DEFAULT_EXPANSION, size), 0, 0);
		this->AddImage(CCircuit::ConcatenateShare(0, holder(1, 0), DEFAULT_EXPANSION, size), 0, 1);

	/*	if (holder.IsInverted())
		{
			
			this->AddImage(CCircuit::ConcatenateShare(0, holder(1, 1), holder(0, 1), DEFAULT_EXPANSION, size), 1, 0);
			
			this->AddImage(CCircuit::ConcatenateShare(0, holder(1, 2), holder(0, 2), DEFAULT_EXPANSION, size), 1, 1);
		}
		else*/
		{
			
			this->AddImage(CCircuit::ConcatenateShare(0, holder(0, 1), holder(1, 1), DEFAULT_EXPANSION, size), 1, 0);
		
			this->AddImage(CCircuit::ConcatenateShare(0, holder(0, 2), holder(1, 2), DEFAULT_EXPANSION, size), 1, 1);
		}

	}


};