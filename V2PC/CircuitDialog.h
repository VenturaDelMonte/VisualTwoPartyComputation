#pragma once
#include "CircuitPanel.h"
#include "Gate.h"
#include "Circuit.h"
#include "CircuitIO.h"

namespace V2PC {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Printing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class CircuitDialog : public System::Windows::Forms::Form
	{
	private: 
		u32 m_circuit_width, m_circuit_height;
		u32 m_screen_width, m_screen_height;
	public:
		CircuitDialog(u32 w, u32 h)
		{
			m_circuit_width = w;
			m_circuit_height = h;
	//		InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CircuitDialog()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		CircuitPanel^ panel;
		List<IntPtr>^ gates;
		SortedDictionary<String^, Bitmap^>^ shares;
	public:
		void UpdateNode(CircuitImageNode* node);
		void OnNodeClicked(void* ptr);
		void Setup(u32 w, u32 h, u32 sw, u32 sh);
		void OnPrintAll(Object^ sender, EventArgs^ e);
		void OnPrintPage(System::Object^ sender, PrintPageEventArgs^ e);
		void PrepareShares(u32 size, u32 expansion);

		void OnFormClosing(Object^ sender, FormClosingEventArgs^ e)
		{
			if (e->CloseReason != CloseReason::UserClosing)
				return;
			this->Hide();
			e->Cancel = true; 
		}

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>

		void InitializeComponent(void)
		{
			gates = gcnew List<IntPtr>();
			shares = gcnew SortedDictionary<String^, Bitmap^>();
			this->SuspendLayout();
			// 
			// CircuitDialog
			// 
			this->FormClosing += gcnew FormClosingEventHandler(this, &CircuitDialog::OnFormClosing);
			this->ClientSize = System::Drawing::Size(min(m_circuit_width + 10, m_screen_width), min(m_circuit_height + 10, m_screen_height));
			this->FormBorderStyle = Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = "CircuitDialog";
			this->Text = "Circuit";
			this->panel = gcnew CircuitPanel(this, m_circuit_width, m_circuit_height, m_screen_width, m_screen_height);
			this->panel->Location = Point(5, 5);
			this->StartPosition = FormStartPosition::CenterScreen;
			this->Controls->Add(this->panel);
			this->ResumeLayout(false);

		}
#pragma endregion


		

	};
}
