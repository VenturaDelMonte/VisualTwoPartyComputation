#include "stdafx.h"
#include "CircuitPanel.h"
namespace V2PC
{
	void CircuitPanel::OnPictureBoxClicked(Object^ sender, EventArgs^ e)
	{
		Point p = PointToClient(Cursor->Position);
		if (this->m_active_index == -1)
            this->GetActiveIndexAtPoint(p);
		if (this->m_active_index > -1 && this->NodeDelegate != nullptr)
			this->NodeDelegate(this->gates[m_active_index].ToPointer());
	}
	void CircuitPanel::OnPictureBoxMouseMove(Object^ sender, MouseEventArgs^ e)
	{
		int index = GetActiveIndexAtPoint(gcnew Point(e->X, e->Y));
		if(m_active_index > -1)
		{
			m_box->Cursor = Cursors::Hand;	
		}
		else
		{
			m_box->Cursor = Cursors::Default;
		}
		m_active_index = index;
	}
	void CircuitPanel::OnPictureBoxMouseLeave(Object^ sender, EventArgs^ e)
	{
		this->m_active_index = -1;
		this->Cursor = Cursors::Default;
	}
	void CircuitPanel::AddImage(Bitmap^ bmp, u32 x, u32 y, void* data)
	{
		if (gates->Count > 0 && data != NULL)
			m_graph_path->SetMarkers();
		Graphics^ g = Graphics::FromImage(m_box->Image);
		g->DrawImage(bmp, float(x), float(m_height - y - 81));
		m_graph_path->AddRectangle(Drawing::Rectangle(x, m_height - y - 81, 80, 80));
		this->gates->Add(IntPtr(data));
	}
	int CircuitPanel::GetActiveIndexAtPoint(Point^ point)
	{
		System::Drawing::Drawing2D::GraphicsPath^ path = gcnew System::Drawing::Drawing2D::GraphicsPath();
		System::Drawing::Drawing2D::GraphicsPathIterator^ iterator = gcnew System::Drawing::Drawing2D::GraphicsPathIterator(m_graph_path);
		iterator->Rewind();
		for (int current=0; current < iterator->SubpathCount; current++)
		{
			iterator->NextMarker(path);
			if (path->IsVisible(Point(point->X, point->Y), m_graphics))
				return current;
		}
		return -1;
	}
}