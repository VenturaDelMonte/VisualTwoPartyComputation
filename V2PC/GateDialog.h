#pragma once

class IGate;
class CRasterImage;

namespace V2PC
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for GateDialog
	/// </summary>
	public ref class GateDialog : public System::Windows::Forms::Form
	{
	public:
		GateDialog(void)
		{
			m_gate = 0;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		void Setup(IGate* g);
		void AddImage(CRasterImage* im, u32 x, u32 y);
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~GateDialog()
		{
			if (components)
			{
				delete components;
			}
		}

	
	private: System::Windows::Forms::Button^  printButton;

	private: System::Windows::Forms::Button^  cancellButton;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::TableLayoutPanel^  panel;


	public:
		System::Void OnSave(Object^ sender, EventArgs^ e);
		System::Void OnPrint(Object^ sender, EventArgs^ e);
		System::Void OnPrintPage(System::Object^ sender, System::Drawing::Printing::PrintPageEventArgs^ e);
	
	private:

		void PrepareShares(u32 size, u32 expansion);

		List<Bitmap^>^ m_ready_shares;

		IGate* m_gate;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->m_ready_shares = gcnew List<Bitmap^>();
			this->printButton = (gcnew System::Windows::Forms::Button());
			this->cancellButton = (gcnew System::Windows::Forms::Button());
			this->panel = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// printButton
			// 
			this->printButton->Location = System::Drawing::Point(489, 418);
			this->printButton->Name = L"printButton";
			this->printButton->Size = System::Drawing::Size(75, 23);
			this->printButton->TabIndex = 1;
			this->printButton->Text = L"Print";
			this->printButton->UseVisualStyleBackColor = true;
			this->printButton->Click += gcnew System::EventHandler(this, &GateDialog::OnPrint);
			// 
			// cancellButton
			// 
			this->cancellButton->Location = System::Drawing::Point(610, 418);
			this->cancellButton->Name = L"cancellButton";
			this->cancellButton->Size = System::Drawing::Size(75, 23);
			this->cancellButton->TabIndex = 3;
			this->cancellButton->Text = L"Save";
			this->cancellButton->UseVisualStyleBackColor = true;
			this->cancellButton->Click += gcnew System::EventHandler(this, &GateDialog::OnSave);
			// 
			// panel
			// 
			this->panel->ColumnCount = 2;
			this->panel->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->panel->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
			this->panel->Location = System::Drawing::Point(40, 26);
			this->panel->Name = L"panel";
			this->panel->RowCount = 2;
			this->panel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->panel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->panel->Size = System::Drawing::Size(645, 375);
			this->panel->TabIndex = 4;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(37, 423);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(103, 13);
			this->label1->TabIndex = 6;
			this->label1->Text = L"Ingrandimento share";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(164, 415);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(110, 20);
			this->textBox1->TabIndex = 7;
			this->textBox1->Text = "8";
			// 
			// GateDialog
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(713, 453);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->panel);
			this->Controls->Add(this->cancellButton);
			this->Controls->Add(this->printButton);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"GateDialog";
			this->Text = L"Gate Dialog";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		
	
	};
};
