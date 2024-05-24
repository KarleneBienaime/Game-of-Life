#pragma once
#include "wx/wx.h" // Include "wx/wx.h" in the MainWindow header file. 
#include "DrawingPanel.h"
#include "Settings.h"

#ifndef MAINWINDOW_H
#define MAINWINDOW_H 

// Create a new class called MainWindow
// This class will inherit from wxFrame
class MainWindow : public wxFrame
{
public:
	MainWindow(); // Add a public constructor
	~MainWindow(); // Add a public destructor

	void SizeChange(wxSizeEvent& event); // window resize event in the main window header file.
	void Grid_Int();
	void gridResize(int _GridSize);

	void OnPlayClicked(wxCommandEvent& event);	// event that is sent when the play button is clicked.
	void OnPauseClicked(wxCommandEvent& event); // pause button event
	void OnNextClicked(wxCommandEvent& event);	// next button event
	void OnTrashClicked(wxCommandEvent& event); // trash button event

	void OpenSettingsDialog(); // settings dialog event
	void OnSettingsMenu(wxCommandEvent& event); // settings menu event

	void OnTimer(wxTimerEvent& event);
	void ShowNeighbors(wxCommandEvent& event);
	void NextGeneration(); 
	void setStatusBar();
	void ShowStatusBar(wxCommandEvent& event);

	void Randomize(int seed); // randomize function
	void OnRandomizeClicked(wxCommandEvent& event); // randomize button event
	void OnRandomizeSeed(wxCommandEvent& event); // randomize seed event

	void OpenSave(wxCommandEvent& event); // open save function
	void SaveGame(wxCommandEvent& event); // save game function
	void SaveAsGame(wxCommandEvent& event); // save as game function
	void ExitGame(wxCommandEvent& event); // exit game function
	void NewGame(wxCommandEvent& event); // new game function

	void LoadGame(const std::string& filename); // load game function
	void SaveAGame(const std::string& filename); // save a game function
	void OnResetClicked(wxCommandEvent& event); // reset button event
	void OnImportClicked(wxCommandEvent& event); // import button event
	void ImportGameBoard(const std::string& filename); // import game function

	void FiniteGameplay(wxCommandEvent& event); // finite gameplay event
	void ToroidalGameplay(wxCommandEvent& event); // toroidal gameplay event
	
	int neighborCount(int row, int col); // counts the number of neighbor cells
	void ShowGrid(wxCommandEvent& event); // show grid event
	void ShowTenGrid(wxCommandEvent& event); // show ten grid event
	static int GridSize;

	wxDECLARE_EVENT_TABLE(); // declares the event table for a class.

private:
	 
	DrawingPanel* Main_DrawingPanel_Ptr = nullptr; // will become a member of MainWindow
	wxBoxSizer* Main_BoxSizer_Ptr = nullptr;   //  It is a box that resizes.
	wxTimer* Time;
	GameSettings settings; // Settings object
	std::vector<std::vector<bool>> GameBoard; // represent the game board
	std::vector<std::vector<int>> neighborsNums; // keeps track of the number of neighbors for each cell

	int TimerSpeed = 50; // 50 milliseconds
	int Generations; // number of generations
	int livingCells; // number of living cells

	static const int One = wxID_HIGHEST + 1; // Define IDs for for parameters in wxMenu*
	static const int Two = wxID_HIGHEST + 2; 
	std::string Filename;
};
#endif // MAINWINDOW_H

