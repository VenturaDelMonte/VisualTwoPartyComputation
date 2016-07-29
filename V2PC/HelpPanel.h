#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace V2PC {

	/// <summary>
	/// Summary for MyUserControl
	/// </summary>
	public ref class HelpPanel : public System::Windows::Forms::UserControl
	{
	public:
		HelpPanel(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~HelpPanel()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^ textBox;

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
			this->SuspendLayout();
			// 
			// MyUserControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Name = L"MyUserControl";
			this->Size = System::Drawing::Size(179, 166);
			this->Load += gcnew System::EventHandler(this, &HelpPanel::MyUserControl_Load);
			this->ResumeLayout(false);
			this->textBox = (gcnew System::Windows::Forms::TextBox());
			// 
			// textBox2
			// 
			this->textBox->Location = System::Drawing::Point(394, 79);
			this->textBox->Name = L"textBox";
			this->textBox->Size = System::Drawing::Size(51, 20);
			this->textBox->TabIndex = 5;
			this->textBox->Text = L"ciasososoasakoaifjdiji asjhaffahfaihfaafa \n dhdshadfu";

			this->Controls->Add(textBox);

		}
#pragma endregion
	private: System::Void MyUserControl_Load(System::Object^  sender, System::EventArgs^  e) {
			 }
	};
}
