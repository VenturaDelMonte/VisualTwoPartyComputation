#include "stdafx.h"
#include "V2PC.h"
#include "MainForm.h"
#include "windows.h"
#include <utility>
#include "Circuit.h"
#include "CircuitIO.h"
#include "HelpPanel.h"

#pragma unmanaged
#include <commctrl.h>
#include "RasterImage.h"
#pragma managed

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "shell32.lib")

extern "C" VGESS_API CircuitIO ValidateFunction(LPCSTR f, u32 image_size);
extern "C" VGESS_API void BuildShares(void* ptr);
extern "C" VGESS_API CRasterImage* SolveCircuit(void* ptr, CAssocVector<u32, bool> in, LPCSTR path);

#define WM_BUILD_DONE (WM_APP + 1)

namespace V2PC
{
	using namespace System::Threading;
	HWND g_window_handle = 0;

	MainForm::~MainForm()
	{
		Application::Idle -= gcnew System::EventHandler(this, &MainForm::OnIdle);
		if (components)
		{
			delete components;
		}
		detail::log_callback = NULL;	
	}

	void MainForm::OnIdle(System::Object ^sender, System::EventArgs ^event_args)
	{
		MSG message;
		do 
		{		
		}
		while (!PeekMessage(&message, NULL, 0, 0, 0));
	}

	void __stdcall progress_callback(float p)
	{
		SendMessage(g_window_handle, PBM_SETPOS, u32(p * 100.0f), 0);
	}

	void MainForm::WndProc(Message% m) 
	{
		switch (m.Msg)
		{
			case PBM_SETPOS:
			{
				UpdateProgressBar(m.WParam.ToInt32());
				break;
			}
			case WM_CIRCUIT:
			{
				m_dialog->UpdateNode((CircuitImageNode*)m.WParam.ToPointer());
				break;
			}
			case WM_REFRESH:
			{
				this->Refresh();
				break;
			}
			case WM_CIRCUIT_DONE:
			{
				m_state = BUILD_DONE;
				Update(m_state);
				break;
			}
			case WM_CIRCUIT_START:
			{
				this->m_dialog->Setup(m.WParam.ToInt32(), m.LParam.ToInt32(), 
					Screen::PrimaryScreen->WorkingArea.Width - 100, Screen::PrimaryScreen->WorkingArea.Height - 100);
				break;
			}
			default: break;
		}
		System::Windows::Forms::Form::WndProc(m);
	}

	

	void MainForm::OnPanelResize(Object^ sender, EventArgs^ e)
	{
		// Resize the buffer, if it is growing
		if (m_bitmap == nullptr || 
			m_bitmap->Width < m_bitmap->Width || 
			m_bitmap->Height < m_bitmap->Height)
		{
			Int32 size = Convert::ToInt32(this->textBox2->Text);
			Bitmap^ newBuffer = gcnew Bitmap(size, size);
			if (m_bitmap != nullptr)
			{
				Graphics^ bufferGrph = Graphics::FromImage(newBuffer);
				bufferGrph->DrawImageUnscaled(m_bitmap, Point::Empty);
			}	
			m_bitmap = newBuffer;
		}
	}

	extern "C" VGESS_API void SetDrawingCallback(OnDrawingFunction cb);
	extern "C" VGESS_API void SetStartDrawingCallback(OnStartDrawingFunction cb);

	void InternalDrawingCallback(CircuitImageNode* node)
	{
		SendMessage(g_window_handle, WM_CIRCUIT, (WPARAM) node, 0);
	}

	void InternalStartDrawingCallback(u32 w, u32 h)
	{
		SendMessage(g_window_handle, WM_CIRCUIT_START, w, h);
	}

	void MainForm::InitLogger()
	{
		g_window_handle = (HWND) this->Handle.ToInt32();
		this->log_callback = gcnew UpdateLogCallback(this, &MainForm::UpdateLog);
//		this->progress_bar_callback = gcnew UpdateProgressBarCallback(this, &MainForm::UpdateProgressBar);
//		SetProgressCallback((ProgressCallback) Marshal::GetFunctionPointerForDelegate(this->progress_bar_callback).ToPointer());
		SetProgressCallback(progress_callback);
		this->toolStripStatusLabel1->TextAlign = ContentAlignment::MiddleLeft;
		SetDrawingCallback(InternalDrawingCallback);
		SetStartDrawingCallback(InternalStartDrawingCallback);
	}
	
	System::Void MainForm::OnGenerateCircuit(System::Object^ sender, System::EventArgs^ e)
	{
		try
		{
			IntPtr str = Marshal::StringToHGlobalAnsi(this->textBox1->Text);
			Int32 size = this->textBox2->Visible ? Convert::ToInt32(this->textBox2->Text) : 0;
			this->panel1->Controls->Clear();
			m_state = GENERATE;
			Update(m_state);
			vec->Clear();
			
			CircuitIO circuit = ValidateFunction(static_cast<char*>(str.ToPointer()), size);
			if(circuit.circuit == NULL)
			{
				return;
			}
			this->panel1->Visible = true;
		
			for(u32 i = 1; i <= circuit.inputs; i++)
			{
				InputPanel^ p = gcnew InputPanel(i);
				vec->Add(p);
				this->panel1->Controls->Add(p);
				p->InputChanged += gcnew OnInputValueChanged(this, &MainForm::OnInputChanged);
			}
			Marshal::FreeHGlobal(str);
			m_circuit = circuit.circuit;
		}
		catch (InternalException const & ex)
		{
			MessageBox::Show(this, gcnew String(ex.what()));
		}
		catch (...)
		{
			MessageBox::Show(this, "Unknown Error");
		}
	}

	void MainForm::OnInputChanged(int index, bool value)
	{
		if(m_state == SOLVE)
		{
			m_state = BUILD_START;
			Update(m_state);
		}
	}


	ref class BuildShareWorker
	{
		public:

			CCircuit* ptr;
			void Execute()
			{
				 BuildShares(ptr);
				 GlobalData::MainFormIstance->OnSharesBuilded();
			}

	};

	void MainForm::build_share_callback()
	{
		BuildShareWorker^ work = gcnew BuildShareWorker;
		work->ptr = m_circuit;
		work->Execute();
		//m_state = BUILD_DONE;
		//Update(m_state);
		SendMessage(g_window_handle, WM_CIRCUIT_DONE, 0, 0);
	}
	
	System::Void MainForm::OnBuildShare(System::Object^ sender, System::EventArgs^ e)
	{		
		UpdateProgressBar(0);
		m_state = BUILD_START;
		Update(m_state);
		ThreadStart^ threadDelegate = gcnew ThreadStart(this, &MainForm::build_share_callback);
		Thread^ newThread = gcnew Thread( threadDelegate );
		newThread->Start();
	}

	ref class CircuitSolverWorker
	{
		
	public:
		List<InputPanel^>^ vec;
		CCircuit* ptr;
		MainForm^ m;
		void Execute()
		{
			GlobalData::MainFormIstance->OnSolved();
			CAssocVector<u32, bool>* user_input = New<CAssocVector<u32, bool>>();
			for each(InputPanel^ p in this->vec)
			{
				u32 a = p->m_index;
				user_input->operator[](a) = p->radioButton2->Checked;
			}
			CRasterImage* res = SolveCircuit(ptr, *user_input, "c:\\");
			m->UpdateResult(res);
			progress_callback(1.0);
			Delete(user_input);
		}

	};
	void MainForm::OnSharesBuilded()
	{
		//SendMessage(g_window_handle, PBM_SETPOS, 100, 0);
		progress_callback(1.0);
		this->ShareButton->Enabled = false;
	}
	void MainForm::OnSolved()
	{
		this->SolveButton->Enabled = false;
		this->ShowButton->Enabled = true;
	}
	extern Bitmap^ RasterToBitmap(CRasterImage* img, u32 expand = 16);

	void MainForm::UpdateResult(CRasterImage* img)
	{
		Log("update result called");
		Bitmap^ bmp = RasterToBitmap(img, 8); // img->getWidth());
		m_bitmap = bmp;
		//this->panel2->Refresh();
		SendMessage(g_window_handle, WM_REFRESH, 0, 0);
		Log("update result end");
	}

	System::Void MainForm::OnSolveCircuit(System::Object^  sender, System::EventArgs^  e) 
	{
		Log("solve called");
		UpdateProgressBar(0);
		CircuitSolverWorker^ w = gcnew CircuitSolverWorker;
		w->vec = vec;
		w->ptr = m_circuit;
		w->m = this;
		m_state = SOLVE;
		Log("solve update");
		Update(m_state);
		Log("solve create thread");
		ThreadStart^ threadDelegate = gcnew ThreadStart(w, &CircuitSolverWorker::Execute);
		Log("solve delegate called");
		Thread^ newThread = gcnew Thread( threadDelegate );
		newThread->Start();
		Log("done");
		
	}

	void MainForm::Update (STATE state)
	{
		switch (state)
		{
			case INIT:
			{
				this->ShowButton->Enabled = false;
				this->ShareButton->Enabled = false;
				this->GenerateButton->Enabled = true;
				this->SolveButton->Enabled = false;
				break;
			}
			case GENERATE:
			{
				this->ShowButton->Enabled = false;
				this->ShareButton->Enabled = true;
				this->GenerateButton->Enabled = false;
				this->SolveButton->Enabled = false;
				this->checkBox1->Enabled = false;
				this->textBox2->Enabled = false;
				break;
			}
			case BUILD_START:
			{
				this->toolStripProgressBar1->Visible = true;
				this->ShareButton->Enabled = false;
				this->GenerateButton->Enabled = false;
				this->ShowButton->Enabled = false;
				this->SolveButton->Enabled = false;
				break;
			}
			case BUILD_DONE:
			{
				this->toolStripProgressBar1->Visible = true;
				this->ShareButton->Enabled = false;
				this->GenerateButton->Enabled = false;
				this->ShowButton->Enabled = true;
				this->SolveButton->Enabled = true;
				break;
			}
			case SOLVE:
			{
				this->ShareButton->Enabled = false;
				this->ShowButton->Enabled = true;
				this->GenerateButton->Enabled = false;
				this->SolveButton->Enabled = false;
				break;
			}
			default:
				break;
		}
	}

	System::Void MainForm::textChange(System::Object^  sender, System::EventArgs^  e) 
	{
		m_state = INIT;
		Update(m_state);
	}
	System::Void MainForm::OnShowButton(System::Object^  sender, System::EventArgs^  e) 
	{
		m_dialog->Show(this);
	}
	System::Void MainForm::OnAbout(System::Object^  sender, System::EventArgs^  e) 
	{
		m_about->Show(this);
	}

}
