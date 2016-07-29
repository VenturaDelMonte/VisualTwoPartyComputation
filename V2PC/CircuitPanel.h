#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Collections::Generic;

namespace V2PC {
	using namespace System::Windows;
	/// <summary>
	/// Summary for MyUserControl
	/// </summary>
	public delegate void OnNodeClickedDelegate(void* data);

	public ref class CircuitPanel : public System::Windows::Forms::UserControl
	{
	private: 
			 Form^ m_parent; 
			 u32 m_width, m_height;
			 u32 m_swidth, m_sheight;
	public:
			 OnNodeClickedDelegate^ NodeDelegate;
	public:
		CircuitPanel(Form^ parent, u32 cw, u32 ch, u32 sw, u32 sh)
		{
			m_parent = parent;
			m_width = cw;
			m_height = ch;
			m_swidth = sw;
			m_sheight = sh;
			m_active_index = -1;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CircuitPanel()
		{
			if (components)
			{
				delete components;
			}
		}
	
		
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

		System::Drawing::Drawing2D::GraphicsPath^ m_graph_path;
		s32 m_active_index;
		Graphics^ m_graphics;
		PictureBox^ m_box;
		List<IntPtr>^ gates;
	protected:
		void OnPictureBoxClicked(Object^ sender, EventArgs^ e);
		void OnPictureBoxMouseMove(Object^ sender, MouseEventArgs^ e);
		void OnPictureBoxMouseLeave(Object^ sender, EventArgs^ e);
		int GetActiveIndexAtPoint(Point^ point);

		Image^ GetBackgroundBitmap(u32 w, u32 h)
		{
			Bitmap^ img = gcnew Bitmap(w, h);
			for (u32 i = 0; i < w; i++)
			{
				for (u32 j = 0; j < h; j++)
				{
					img->SetPixel(i, j, Color::White);
				}
			}
			return img;
		}
	public:
		void AddImage(Bitmap^ bmp, u32 x, u32 y, void* data);
	protected:

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			this->m_graph_path = gcnew System::Drawing::Drawing2D::GraphicsPath();
			this->m_graph_path->FillMode = System::Drawing::Drawing2D::FillMode::Winding;

			this->m_box = gcnew System::Windows::Forms::PictureBox();
			this->m_box->Dock = System::Windows::Forms::DockStyle::Fill;
			this->m_box->Name = "pictureBox";
			this->m_box->Size = System::Drawing::Size(m_width, m_height);
			this->m_box->TabIndex = 0;
			this->m_box->TabStop = false;
			this->m_box->Click += gcnew System::EventHandler(this, &CircuitPanel::OnPictureBoxClicked);
			this->m_box->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &CircuitPanel::OnPictureBoxMouseMove);
			this->m_box->MouseLeave += gcnew System::EventHandler(this, &CircuitPanel::OnPictureBoxMouseLeave); 
			this->m_box->Image = GetBackgroundBitmap(m_width, m_height);
		//	this->m_box->BackgroundImage = GetBackgroundBitmap(m_width, m_height);

			this->m_graphics = Graphics::FromHwnd(this->m_box->Handle);
			this->gates = gcnew List<IntPtr>();

		//	u32 c = m_width / 80, r = m_height / 80;

			
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
			this->Controls->AddRange(gcnew array<Control^> { this->m_box } );
			this->Name = L"CircuitPanel";
			this->ClientSize = System::Drawing::Size(min(m_width, m_swidth), min(m_height, m_sheight));
			this->AutoScrollMinSize = System::Drawing::Size(min(m_width, m_width), min(m_height, m_height));
			this->m_parent->AutoSize = false;
			this->AutoScroll = true;
			this->m_parent->Refresh();
			this->ResumeLayout(false);
				

		}
#pragma endregion
	};
}
