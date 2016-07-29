#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace V2PC {

	public delegate void OnInputValueChanged(int index, bool value);

	/// <summary>
	/// Summary for MyUserControl
	/// </summary>
	public ref class InputPanel : public System::Windows::Forms::GroupBox
	{
	public:
		InputPanel(int i)
		{
			m_index = i;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			setLabel(i);
		}
		void setLabel(int);
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~InputPanel()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::RadioButton^  radioButton1;
	public: System::Windows::Forms::RadioButton^  radioButton2;
	public: 
		int m_index;
		OnInputValueChanged^ InputChanged;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e)
		{
			if (InputChanged != nullptr)
				InputChanged(m_index, radioButton2->Checked);
		}
	
	private:
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
			this->InputChanged = nullptr;
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(15, 11);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(40, 13);
			this->label1->TabIndex = 0;
			
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Location = System::Drawing::Point(72, 11);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(31, 17);
			this->radioButton1->TabIndex = 1;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"0";
			this->radioButton1->UseVisualStyleBackColor = true;
			this->radioButton1->CheckedChanged += gcnew EventHandler(this, &InputPanel::OnValueChanged);
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(120, 11);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(31, 17);
			this->radioButton2->TabIndex = 2;
			this->radioButton2->TabStop = true;
			this->radioButton2->Text = L"1";
			this->radioButton2->UseVisualStyleBackColor = true;
			this->radioButton2->CheckedChanged += gcnew EventHandler(this, &InputPanel::OnValueChanged);
			// 
			// InputPanel
			// 
			
			this->Controls->Add(this->radioButton2);
			this->Controls->Add(this->radioButton1);
			this->Controls->Add(this->label1);
			this->Name = L"InputPanel";
			this->Size = System::Drawing::Size(155, 35);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
	};
}
