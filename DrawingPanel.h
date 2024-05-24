#pragma once
#include "wx/wx.h" // Include "wx/wx.h" in the MainWindow header file.
#include "Settings.h"

class DrawingPanel : public wxPanel // Creates a DrawingPanel class that inherits from the wxPanel class.
{
public:
    DrawingPanel(wxFrame* parent, std::vector<std::vector<bool>>& GameBoard, std::vector<std::vector<int>>& neighborsNums, GameSettings* settings); // constructor
    ~DrawingPanel(); // destructor for the DrawingPanel class.

    void OnPaint(wxPaintEvent& event); // wxPaintEvent& is an event that is sent when a window is redrawn.
    void ResizePanel(wxSizeEvent& event); // represents the size of a window.
    void OnMouseUp(wxMouseEvent& event); // event that is sent when the mouse is clicked.

    wxDECLARE_EVENT_TABLE(); // declares the event table for a class.

private:
    std::vector<std::vector<bool>>& Stoarge; // reference to the GameBoard vector.
    std::vector<std::vector<int>>& neighborsCount; // keeps track of the number of neighbors for each cell
    GameSettings* settings; // Settings object
};

