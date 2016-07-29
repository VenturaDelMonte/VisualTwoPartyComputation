#pragma once
#include "Debug.h"
#using <mscorlib.dll>
#include "InputPanel.h"
#include "CircuitDialog.h"
#include "HelpForm.h"

//#include "..\VisualGESS++\VisualGESSLib++\Circuit.h"

class VGESS_API CRasterImage;

#define WM_REFRESH WM_USER + 250
#define WM_CIRCUIT WM_USER + 251
#define WM_CIRCUIT_START WM_USER + 252
#define WM_CIRCUIT_DONE WM_USER + 253

enum STATE
{
	INIT,
	GENERATE,
	BUILD_START,
	BUILD_DONE,
	SOLVE,
	SHOW
};
namespace V2PC 
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Runtime::InteropServices;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;
	/// <summary>
	/// Summary for UpdateLogCallback
	/// </summary>
	public delegate void UpdateLogCallback(LPCSTR str);
//	public delegate void UpdateProgressBarCallback(float progress);

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			vec = gcnew List<InputPanel^>();
			InitializeComponent();
			InitLogger();
			m_state = INIT;
			Update(m_state);
			m_bitmap = nullptr;
			OnPanelResize(this, nullptr);
			Application::Idle += gcnew System::EventHandler(this, &MainForm::OnIdle);
		}

		void UpdateLog(LPCSTR str)
		{
			if (!toolStripStatusLabel1->Visible)
				toolStripStatusLabel1->Visible = true;
			toolStripStatusLabel1->Text = String::Format(" :: {0} ", gcnew String(str));
		}

		void UpdateProgressBar(int progress)
		{
			this->toolStripProgressBar1->Value = progress;
		}

		void InitLogger();
		
		void OnIdle(System::Object^ sender, System::EventArgs^ event_args);
		virtual void WndProc(Message% m) override;
		void UpdateResult(CRasterImage* img);
		void OnInputChanged(int index, bool value);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm();
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;
	private: System::Windows::Forms::ToolStripProgressBar^  toolStripProgressBar1;

	public: 
		UpdateLogCallback^ log_callback;
	//	UpdateProgressBarCallback^ progress_bar_callback;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Button^  GenerateButton;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox2;			 
	private: System::Windows::Forms::ToolStripMenuItem^  quitToolStripMenuItem;	
	private: System::Windows::Forms::ToolStripMenuItem^  aToolStripMenuItem;	
	private: CCircuit* m_circuit;
	private: STATE m_state;
	private: CircuitDialog^ m_dialog;
	private: HelpForm^ m_about;

	private: List<InputPanel^> ^vec;
	private: System::Windows::Forms::Button^  ShowButton;
	private: System::Windows::Forms::FlowLayoutPanel^  panel1;
	private: System::Windows::Forms::Button^  ShareButton;
	private: System::Windows::Forms::Button^  SolveButton;
	private: System::Windows::Forms::Panel^  panel2;
	private: Bitmap^ m_bitmap;

	public:
			CCircuit& Circuit()
			{
				return *m_circuit;
			}
	private: 
	
		void OnPanelResize(Object^ sender, EventArgs^ e);

		

		void OnPanelPaint(Object^ sender, PaintEventArgs^ e)
		{
			e->Graphics->DrawImageUnscaled(m_bitmap, Point::Empty);
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

		void build_share_callback();

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->GenerateButton = (gcnew System::Windows::Forms::Button());
			this->ShareButton = (gcnew System::Windows::Forms::Button());
			this->ShowButton = (gcnew System::Windows::Forms::Button());
			this->SolveButton = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->panel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->quitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripProgressBar1 = (gcnew System::Windows::Forms::ToolStripProgressBar());
			this->m_dialog = gcnew CircuitDialog(0, 0);
			this->m_about = gcnew HelpForm();
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->ShareButton);
			this->groupBox1->Controls->Add(this->ShowButton);
			this->groupBox1->Controls->Add(this->textBox2);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->checkBox1);
			this->groupBox1->Controls->Add(this->GenerateButton);
			this->groupBox1->Controls->Add(this->textBox1);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(15, 32);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(765, 166);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Building";
		//	this->groupBox1->Enter += gcnew System::EventHandler(this, &MainForm::groupBox1_Enter);
			// 
			// button2
			// 
			this->ShareButton->Enabled = false;
			this->ShareButton->Location = System::Drawing::Point(332, 118);
			this->ShareButton->Name = L"button2";
			this->ShareButton->Size = System::Drawing::Size(84, 23);
			this->ShareButton->TabIndex = 7;
			this->ShareButton->Text = L"Build Shares";
			this->ShareButton->UseVisualStyleBackColor = true;
			this->ShareButton->Click += gcnew System::EventHandler(this, &MainForm::OnBuildShare);
			// 
			// button3
			// 
			this->ShowButton->Enabled = false;
			this->ShowButton->Location = System::Drawing::Point(474, 118);
			this->ShowButton->Name = L"button3";
			this->ShowButton->Size = System::Drawing::Size(82, 23);
			this->ShowButton->TabIndex = 6;
			this->ShowButton->Text = L"Show Circuit";
			this->ShowButton->UseVisualStyleBackColor = true;
			this->ShowButton->Click += gcnew System::EventHandler(this, &MainForm::OnShowButton);
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(394, 79);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(51, 20);
			this->textBox2->TabIndex = 5;
			this->textBox2->Text = L"8";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(293, 82);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(86, 13);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Size Image";
			// 
			// checkBox1
			// 
			this->checkBox1->Location = System::Drawing::Point(36, 78);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(171, 17);
			this->checkBox1->TabIndex = 3;
			this->checkBox1->Text = L"Automatic Size";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &MainForm::checkBox1_Checked);
			// 
			// button1
			// 
			this->GenerateButton->Location = System::Drawing::Point(166, 118);
			this->GenerateButton->Name = L"button1";
			this->GenerateButton->Size = System::Drawing::Size(108, 23);
			this->GenerateButton->TabIndex = 2;
			this->GenerateButton->Text = L"Generate Circuit";
			this->GenerateButton->UseVisualStyleBackColor = true;
			this->GenerateButton->Click += gcnew System::EventHandler(this, &MainForm::OnGenerateCircuit);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(93, 24);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(519, 20);
			this->textBox1->TabIndex = 1;
			this->textBox1->Text = "";
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &MainForm::textChange);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(33, 27);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(54, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Function: ";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->panel2);
			this->groupBox2->Controls->Add(this->SolveButton);
			this->groupBox2->Controls->Add(this->panel1);
			this->groupBox2->Location = System::Drawing::Point(15, 204);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(765, 234);
			this->groupBox2->TabIndex = 1;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Solving";

			// 
			// panel2
			// 
			this->panel2->Location = System::Drawing::Point(433, 19);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(320, 165);
			this->panel2->TabIndex = 2;
			this->panel2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainForm::OnPanelPaint);
			this->panel2->Resize += gcnew System::EventHandler(this, &MainForm::OnPanelResize);
			// 
			// button4
			// 
			this->SolveButton->Enabled = false;
			this->SolveButton->Location = System::Drawing::Point(548, 187);
			this->SolveButton->Name = L"button4";
			this->SolveButton->Size = System::Drawing::Size(93, 31);
			this->SolveButton->TabIndex = 1;
			this->SolveButton->Text = L"Solve";
			this->SolveButton->UseVisualStyleBackColor = true;
			this->SolveButton->Click += gcnew System::EventHandler(this, &MainForm::OnSolveCircuit);
			// 
			// panel1
			// 
			this->panel1->AutoScroll = true;
			this->panel1->Location = System::Drawing::Point(18, 19);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(398, 199);
			this->panel1->TabIndex = 0;
			this->panel1->Visible = false;
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fileToolStripMenuItem, 
				this->aboutToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(790, 24);
			this->menuStrip1->TabIndex = 2;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->quitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->aToolStripMenuItem});
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(99, 22);
			this->aboutToolStripMenuItem->Text = L"About";
			
			//
			//aToolStripMenuItem
			//
			//
			this->aToolStripMenuItem->Name = L"aToolStripMenuItem";
			this->aToolStripMenuItem->Size = System::Drawing::Size(99, 22);
			this->aToolStripMenuItem->Text = L"About V2PC++";
			this->aToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::OnAbout);
		
			
			//
			//quitToolStripMenuItem
			//
			this->quitToolStripMenuItem->Name = L"quitToolStripMenuItem";
			this->quitToolStripMenuItem->Size = System::Drawing::Size(131, 22);
			this->quitToolStripMenuItem->Text = L"Quit";
			this->quitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::OnQuit);
			
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->toolStripStatusLabel1, 
				this->toolStripProgressBar1});
			this->statusStrip1->Location = System::Drawing::Point(0, 448);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(790, 22);
			this->statusStrip1->TabIndex = 3;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->AutoSize = false;
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(550, 17);
			// 
			// toolStripProgressBar1
			// 
			this->toolStripProgressBar1->Name = L"toolStripProgressBar1";
			this->toolStripProgressBar1->Size = System::Drawing::Size(200, 16);
			this->toolStripProgressBar1->Visible = false;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(790, 470);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MainForm";
			this->Text = L"V2PC++";
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion


		 //validate function
private: System::Void OnGenerateCircuit(System::Object^  sender, System::EventArgs^  e);


		 //if checked, size image
private: System::Void checkBox1_Checked(System::Object^  sender, System::EventArgs^  e) {
			if(this->checkBox1->Checked)
			{
				this->label2->Visible = false;
				this->textBox2->Visible = false;
			}
			else
			{
				this->label2->Visible = true;
				this->textBox2->Visible = true;
			}
			//this->checkBox1->Enabled = false;
			//this->textBox2->Enabled = false;
		 }
		 

private: System::Void OnBuildShare(System::Object^  sender, System::EventArgs^  e);
private: System::Void OnAbout(System::Object^  sender, System::EventArgs^  e);		 
private: System::Void OnQuit(System::Object^  sender, System::EventArgs^  e){ this->Close();}		 
public: System::Void OnShowButton(System::Object^  sender, System::EventArgs^  e) ;
public: System::Void textChange(System::Object^  sender, System::EventArgs^  e);
private: System::Void Update(STATE state);
public: System::Void OnSharesBuilded();
public: System::Void OnSolved();
private: System::Void OnSolveCircuit(System::Object^  sender, System::EventArgs^  e);
};
}
