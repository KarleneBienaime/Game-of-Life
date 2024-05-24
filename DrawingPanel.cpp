#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"
#include "MainWindow.h"
#include "SettingsDialog.h"
#include "Settings.h"


wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
EVT_PAINT(DrawingPanel::OnPaint)  // OnPaint -> paint event
EVT_SIZE(DrawingPanel::ResizePanel)  // ResizePanel -> size event
EVT_LEFT_UP(DrawingPanel::OnMouseUp)  // OnMouseUp -> left click mouse event
wxEND_EVENT_TABLE()

 // The wxAutoBufferedPaintDC class is a wxBufferedPaintDC class that automatically creates a wxBitmap object the size of the window and draws on it.
DrawingPanel::DrawingPanel(wxFrame* parent, std::vector<std::vector<bool>>& GameBoard, std::vector<std::vector<int>>& neighborsNums, GameSettings* settings) : wxPanel(parent), Stoarge(GameBoard), neighborsCount(neighborsNums), settings(settings) // constructor
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT); // Set the background style to wxBG_STYLE_PAINT
	this->SetBackgroundColour(wxColour(*wxWHITE)); // Set the background color to white

}
DrawingPanel::~DrawingPanel()
{
	// Create an empty destructor for now.
}
void DrawingPanel::OnPaint(wxPaintEvent& event)
{
	wxAutoBufferedPaintDC dc(this); // auto creates a Bitmap object the size of the window and draws on it.
	dc.Clear(); // Clears the window

	std::unique_ptr<wxGraphicsContext> gc(wxGraphicsContext::Create(dc)); // Graphics context is used to draw graphics on a wxDC object (wxAutoBufferedPaintDC)

	if (!gc) return; // Exit if failed

	wxSize panelSize = GetSize(); // Get the size of the panel
	double cellWidth = panelSize.x / Stoarge[0].size();
	double cellHeight = panelSize.y / Stoarge.size();

	wxColour livingColor = settings->Get_LivingCellColor();  // Get the living cell color from settings
	wxColour deadColor = settings->Get_DeadCellColor();  // Get the dead cell color from settings
	wxBrush livingBrush(livingColor);
	wxBrush deadBrush(deadColor);

	for (int i = 0; i < Stoarge.size(); ++i) // does first loop for the size of the storage vector = gameboard
	{
		for (int j = 0; j < Stoarge[i].size(); ++j) // second loop  *make sure to change from ints to storage size*
		{
			if (settings->ShowGrid)
			{
				gc->SetPen(*wxBLACK_PEN);
			}
			else
			{
				gc->SetPen(*wxTRANSPARENT_PEN);
			}
			gc->SetBrush(Stoarge[i][j] ? livingBrush : deadBrush);
			gc->DrawRectangle(i * cellWidth, j * cellHeight, cellWidth, cellHeight); // Draw a rectangles

		}
	}

	if (settings->showNeighborCount)
	{
		wxFontInfo font(12); // Create a font object	
		gc->SetFont(font, *wxBLACK); // Set the font to black

		for (int i = 0; i < Stoarge.size(); ++i)
		{
			for (int j = 0; j < Stoarge[i].size(); ++j)
			{
				int Num = neighborsCount[i][j];

				if (Num > 0)
				{
					wxString Text = wxString::Format("%d", Num);
					double textWidth, textHeight;
					gc->GetTextExtent(Text, &textWidth, &textHeight);

					// Calculate the correct X and Y positions to draw the text
					double X = i * cellWidth + (cellWidth - textWidth) / 2.0;
					double Y = j * cellHeight + (cellHeight - textHeight) / 2.0;

					gc->DrawText(Text, X, Y); // Draw the text
				}
			}
		}
	}

	int TenLines = Stoarge[0].size() / 10;
	if (settings->ShowTenGrid)
	{
		wxPen pen(*wxBLACK, 2);
		dc.SetPen(pen);

		for (int i = 0; i < TenLines; i++)
		{
			wxPoint vStart((i + 1) * 10 * cellWidth, 0);
			wxPoint vEnd((i + 1) * 10 * cellWidth, panelSize.y);
			dc.DrawLine(vStart, vEnd);

			wxPoint hStart(0, (i + 1) * 10 * cellHeight);
			wxPoint hEnd(panelSize.x, (i + 1) * 10 * cellHeight);
			dc.DrawLine(hStart, hEnd);
		}
	}
}
void DrawingPanel::ResizePanel(wxSizeEvent& event) // resize the panel to the size of the window *IMPORTANT*
{
	wxSize size = GetSize(); 
	wxPanel::SetSize(size); // Call to wxpanel not drawingpanel parent
	Refresh();
}
void DrawingPanel::OnMouseUp(wxMouseEvent& event) // mouse click and changes the color of the cell
{
	float  x = event.GetX(); // Get the x position of the mouse
	float  y = event.GetY(); // Get the y position of the mouse

	wxSize size = GetSize();
	float  cellWidth = (float)size.x / Stoarge[0].size();
	float  cellHeight = (float)size.y / Stoarge.size();

	int row = (int)(y / cellHeight);
	int col = (int)(x / cellWidth);

	Stoarge[col][row] = !Stoarge[col][row]; // corrected postion of mouse click

	Refresh();
}