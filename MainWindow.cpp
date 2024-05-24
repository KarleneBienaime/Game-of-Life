#include "MainWindow.h"
#include "DrawingPanel.h"
#include "SettingsDialog.h"
#include <wx/numdlg.h> // wxGetNumberFromUser
#include <string>
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"
#include <fstream>  // need for open/close file file operations
#include <algorithm> //  need for std::min
#include <wx/filedlg.h>  // For wxFileDialog

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)

	EVT_SIZE(MainWindow::SizeChange)				// WINDOW RESIZE
	//-----------------------------------------------------------
	EVT_MENU(10001, MainWindow::OnPlayClicked)
	EVT_MENU(10002, MainWindow::OnPauseClicked)
	EVT_MENU(10003, MainWindow::OnNextClicked)
	EVT_MENU(10004, MainWindow::OnTrashClicked)			
	EVT_MENU(10005, MainWindow::OnResetClicked)
	EVT_MENU(10006, MainWindow::OnImportClicked)		// PLAY, PAUSE, NEXT, TRASH BUTTONS
	//-----------------------------------------------------------
	EVT_TIMER(wxID_ANY, MainWindow::OnTimer)			// TIMER
	//-----------------------------------------------------------
	EVT_MENU(54321, MainWindow::OnSettingsMenu)
	EVT_MENU(12345, MainWindow::ShowNeighbors)			
	EVT_MENU(22345, MainWindow::ShowGrid)
	EVT_MENU(32345, MainWindow::ShowTenGrid)
	EVT_MENU(42345, MainWindow::ShowStatusBar)			// SETTINGS, SHOW GRID, SHOW 10X10 GRID, SHOW HUD, SHOW NEIGHBORS COUNT
	//-----------------------------------------------------------
	EVT_MENU(One, MainWindow::OnRandomizeClicked)
	EVT_MENU(Two, MainWindow::OnRandomizeSeed)			// RANDOMIZE, RANDOMIZE WITH SEED
	//-----------------------------------------------------------
	EVT_MENU(20001, MainWindow::NewGame)
	EVT_MENU(20002, MainWindow::OpenSave)
	EVT_MENU(20003, MainWindow::SaveGame)
	EVT_MENU(20004, MainWindow::SaveAsGame)
	EVT_MENU(20005, MainWindow::ExitGame)				// NEW, OPEN, SAVE, SAVE AS, EXIT
	//-----------------------------------------------------------
	EVT_MENU(20001, MainWindow::FiniteGameplay)
	EVT_MENU(20002, MainWindow::ToroidalGameplay)		// FINITE, TOROIDAL
wxEND_EVENT_TABLE()
//-------------------CONSTRUCTER-----------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(30, 30), wxSize(800, 600))
// wxFrame (wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize)
{
	wxStatusBar* statusBar = CreateStatusBar(); //  status bar at the bottom of the window
	wxToolBar* toolBar = CreateToolBar(); // tool bar at the top of the window
	wxMenuBar* OptionsBar = new wxMenuBar; 	// MAIN menu bar

//-------
	wxMenu* viewMenu = new wxMenu; //VIEW

	viewMenu->AppendCheckItem(12345, "Show Neighbors Count"); 	// Add Show neighbors count-> View menu
	viewMenu->AppendCheckItem(22345, "Show Grid");
	viewMenu->Check(22345, true);
	viewMenu->AppendCheckItem(32345, "Show 10x10 Grid");
	viewMenu->AppendCheckItem(42345, "Show HUD");
	viewMenu->Check(42345, true);

//-------
	wxMenu* randomizeMenu = new wxMenu; // RANDOMIZE

	randomizeMenu->Append(One, "Randomize");					// Add randomize->Randomize menu
	randomizeMenu->Append(Two, "Randomize with seed");			// Add randomize with->Randomize menu
//-------
	wxMenu* fileMenu = new wxMenu; //FILE

	fileMenu->Append(20001, "New");
	fileMenu->Append(20002, "Open");
	fileMenu->Append(20003, "Save");
	fileMenu->Append(20004, "Save As");
	fileMenu->Append(10005, "Reset Settings");
	fileMenu->Append(10006, "Import");
	fileMenu->Append(20005, "Exit");
//-------
	wxMenu* optionsMenu = new wxMenu; //OPTIONS

	optionsMenu->AppendSubMenu(randomizeMenu, "Randomize");	  // Add randomize->Options menu
	optionsMenu->AppendCheckItem(20001, "Finite");
	optionsMenu->AppendCheckItem(20002, "Toroidal");
//-------
	OptionsBar->Append(fileMenu, "File"); 						// Add parent File menu to the menu bar
	OptionsBar->Append(optionsMenu, "Options");                 // Add parent Options menu to the menu bar
	OptionsBar->Append(viewMenu, "View");						// Add parent View menu to the menu bar
	SetMenuBar(OptionsBar);										// Refresh the menu bar (FILE, OPTIONS, VIEW)

	Time = new wxTimer(this, wxID_ANY); // timer

//-------
	wxBitmap playIcon(play_xpm);	// PLAY icon
	toolBar->AddTool(10001, "Play", playIcon);		// add icon -> tool bar
	wxBitmap pauseIcon(pause_xpm);	// PAUSE icon
	toolBar->AddTool(10002, "Pause", pauseIcon);	// add icon -> tool bar
	wxBitmap nextIcon(next_xpm);	// NEXT icon
	toolBar->AddTool(10003, "Next", nextIcon);		// add icon -> tool bar
	wxBitmap trashIcon(trash_xpm);	// TRASH icon
	toolBar->AddTool(10004, "Trash", trashIcon);	// add icon -> tool bar
//-------
		toolBar->Realize(); // realize the tool bar

	// Create drawingPanel and make it a "child" of MainWindow
	Main_DrawingPanel_Ptr = new DrawingPanel(this, GameBoard, neighborsNums, &settings);

	Main_BoxSizer_Ptr = new wxBoxSizer(wxVERTICAL);
	Main_BoxSizer_Ptr->Add(Main_DrawingPanel_Ptr, 1, wxEXPAND | wxALL); //
	this->SetSizer(Main_BoxSizer_Ptr); //

	Grid_Int(); // need to initialize the grid before using it

	this->Layout(); // update the layout of the window
}
//-------------------DECONSTRUCTER-----------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
	// added to stop memory leak from wxtimer
	if (Time->IsRunning()) {
		Time->Stop();
	}

	// Delete timer
	delete Time;
	Time = nullptr;
	
	// NOTE: The MainWindow is the parent of everything, so it will automatically clean up its children
}
//----------------------WINDOW RESIZERS------------------------------------------------------------------------------------------------------------
void MainWindow::SizeChange(wxSizeEvent& event)
{
	wxSize Window_Size = event.GetSize();
	if (Main_DrawingPanel_Ptr != nullptr) // if the drawing panel is not null
	{
		Main_DrawingPanel_Ptr->SetSize(Window_Size);
	}
	event.Skip(); // find handler for this event (rememeber to add to notes)
}
void MainWindow::Grid_Int()
{
	GameBoard.resize(settings.gridSize);

	for (int i = 0; i < settings.gridSize; i++)
	{
		GameBoard[i].resize(settings.gridSize); // all elements in the *gameboard* will be initialized
	}

	neighborsNums.resize(settings.gridSize);

	for (int i = 0; i < settings.gridSize; i++)
	{
		neighborsNums[i].resize(settings.gridSize, 0); // all elements in the *neighborsNums* will be initialized
	}
}
void MainWindow::gridResize(int _GridSize)
{
	settings.gridSize = _GridSize;
	Grid_Int();
}
//----------------------ICON BUTTONS---------------------------------------------------------------------------------------------------------------
void MainWindow::OnPlayClicked(wxCommandEvent& event) 
{
	Time->Start(TimerSpeed); // start timer
}
void MainWindow::OnPauseClicked(wxCommandEvent& event)
{
	Time->Stop(); // stop timer
}
void MainWindow::OnNextClicked(wxCommandEvent& event)
{
	NextGeneration(); 
}
void MainWindow::OnTrashClicked(wxCommandEvent& event)
{
	Generations = 0; // reset the number of generations
	livingCells = 0; // empty of living cells

	for (int i = 0; i < GameBoard.size(); ++i)  // touch all cells
	{
		for (int j = 0; j < GameBoard[0].size(); ++j)
		{
			GameBoard[i][j] = false; // kill each cell
		}
	}

	Main_DrawingPanel_Ptr->Refresh(); // clear the drawing panel
}
void MainWindow::OnTimer(wxTimerEvent& event)
{
	NextGeneration();
}
void MainWindow::ShowNeighbors(wxCommandEvent& event)
{
	settings.showNeighborCount = !settings.showNeighborCount; 
	Main_DrawingPanel_Ptr->Refresh();
}
void MainWindow::ShowStatusBar(wxCommandEvent& event)
{
	settings.ShowHUD = !settings.ShowHUD;
	if (settings.ShowHUD == false)
	{
		std::string Update = "HUD is hidden";
		SetStatusText(Update);
	}
	else
	{
		setStatusBar();
	}
}
//----------------------LOAD/SAVE GAME-------------------------------------------------------------------------------------------------------------
void MainWindow::NewGame(wxCommandEvent& event)
{
	Grid_Int();  // Clear the grid
	Filename = "";
}
void MainWindow::LoadGame(const std::string& filename)
{
	std::ifstream infile(Filename);
	std::string line;
	if (infile.is_open())
	{
		std::getline(infile, line);
		int gridSize = std::stoi(line);
		gridResize(gridSize);

		for (int i = 0; i < GameBoard.size(); ++i)
		{
			for (int j = 0; j < GameBoard[0].size(); ++j)
			{
				std::getline(infile, line);
				GameBoard[i][j] = (line == "1");
			}
		}
		infile.close();
	}
}
void MainWindow::SaveAGame(const std::string& filename)
{
	std::ofstream outfile(filename);

	if (outfile.is_open())
	{
		outfile << settings.gridSize << std::endl;

		for (int i = 0; i < GameBoard.size(); ++i)
		{
			for (int j = 0; j < GameBoard[0].size(); ++j)
			{
				outfile << GameBoard[i][j] << std::endl;
			}
		}
		outfile.close();
	}
}
void MainWindow::OpenSave(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, _("Open .cells file"), "", "",
		"CELLS files (*.cells)|*.cells", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL) {
		return; // User cancelled
	}
	Filename = openFileDialog.GetPath().ToStdString();
	LoadGame(Filename);  // Call to LoadFile to actually load the grid
	Main_DrawingPanel_Ptr->Refresh();
}
void MainWindow::SaveAsGame(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog(this, _("Save .cells file"), "", "",
		"CELLS files (*.cells)|*.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL) {
		return; // User cancelled
	}
	Filename = saveFileDialog.GetPath().ToStdString();
	SaveAGame(Filename);  // Call to SaveFile to actually save the grid
}
void MainWindow::ExitGame(wxCommandEvent& event)
{
	this->Close(); // close the window
}
void MainWindow::SaveGame(wxCommandEvent& event)
{
	if (Filename.empty()) 
	{
		SaveAsGame(event);  // If no current filename, use Save As dialog
	}
	else 
	{
		SaveGame(event);  // Call to SaveFile to actually save the grid
	}
}
//----------------------------SETTINGS---------------------------------------------------------------------------------------------------------------
void MainWindow::OnResetClicked(wxCommandEvent& event)
{
	settings.Reset(); // reset the settings (located in settings.h)
	settings.Save();
}
void MainWindow::OnImportClicked(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(
		this, _("Import game board"), "", "",
		"Cells files (*.cells)|*.cells",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST
	);

	if (openFileDialog.ShowModal() == wxID_CANCEL) return;

	std::string filename = openFileDialog.GetPath().ToStdString();
	ImportGameBoard(filename);
}
void MainWindow::ImportGameBoard(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		// Added to handle error: could not open file
		return;
	}

	std::vector<std::vector<bool>> importedBoard;	// Temporary board to store the imported data

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty() || line[0] == '!') // Skip empty AND comment lines
			continue;

		std::vector<bool> row;
		for (char c : line)
		{
			if (c == '*') // Living cell
				row.push_back(true);
			else if (c == '.') // Dead cell
				row.push_back(false);
		}
		importedBoard.push_back(row);
	}
	file.close();

	// Loop to update GameBoard based on the imported data
	for (int i = 0; i < std::min(GameBoard.size(), importedBoard.size()); ++i)
	{
		for (int j = 0; j < std::min(GameBoard[i].size(), importedBoard[i].size()); ++j)
		{
			GameBoard[i][j] = importedBoard[i][j];
		}
	}

	Main_DrawingPanel_Ptr->Refresh();
}
void MainWindow::OpenSettingsDialog()
{
	SettingsDialog settingsDialog(this, &settings);

	if (settingsDialog.ShowModal() == wxID_OK)
	{
		// button clicked-> apply settings
		Grid_Int();
		Main_DrawingPanel_Ptr->Refresh();
		settings.Save();
	}
}
void MainWindow::OnSettingsMenu(wxCommandEvent& event)
{
	SettingsDialog settingsDialog(this, &settings);

	if (settingsDialog.ShowModal() == wxID_OK)
	{
		// button clicked-> apply settings
		Grid_Int();
		Main_DrawingPanel_Ptr->Refresh();
		settings.Save();
	}
}
void MainWindow::setStatusBar()
{
		std::string Generations_Str = std::to_string(Generations);
		std::string livingCell_Str = std::to_string(livingCells);
		std::string Update = "Generations: " + Generations_Str + ", Living Cells: " + livingCell_Str + ", Boundary type: " + (settings.Toroidal ? "Toroidal" : "Finite") + ", Universe: " + std::to_string(settings.gridSize);
		SetStatusText(Update);
}
int MainWindow::neighborCount(int row, int col)
{
	int count = 0;
	int gridSize = GameBoard.size();

	for (int i = row - 1; i <= row + 1; ++i)
	{
		for (int j = col - 1; j <= col + 1; ++j)
		{
			if (i == row && j == col) continue; // Skip the cell itself

			int toroidal_i = (i + gridSize) % gridSize;
			int toroidal_j = (j + gridSize) % gridSize;

			// finite grid = skip the out-of-bounds cells
			if (settings.Finite && (i < 0 || j < 0 || i >= gridSize || j >= gridSize))
			{
				continue;
			}

			// if grid is toroidal, otherwise use original indices
			int final_i = settings.Finite ? i : toroidal_i;
			int final_j = settings.Finite ? j : toroidal_j;

			if (GameBoard[final_i][final_j]) ++count;
		}
	}
	return count;
}
void MainWindow::ShowGrid(wxCommandEvent& event)
{
	settings.ShowGrid = !settings.ShowGrid;
	Main_DrawingPanel_Ptr->Refresh();  
	settings.Save();  
}
void MainWindow::ShowTenGrid(wxCommandEvent& event)
{
	settings.ShowTenGrid = !settings.ShowTenGrid;
	Main_DrawingPanel_Ptr->Refresh();
	settings.Save(); 
}
void MainWindow::NextGeneration()
{
	std::vector<std::vector<bool>> sandbox; // temporary gameboard
	sandbox.resize(GameBoard.size()); // resize the sandbox to the size of the gameboard

	int livingCells = 0; // number of live cells
	int neighbors = 0; // number of neighbors

	neighborsNums.clear();
	neighborsNums.resize(GameBoard.size());

	for (int i = 0; i < GameBoard.size(); ++i) // loop through the rows
	{
		for (int j = 0; j < GameBoard[0].size(); ++j) // loop through the columns
		{
			neighbors = neighborCount(i, j); // neighbor [i] = row, [j] = column
			neighborsNums[i].push_back(neighbors); // add the number of neighbors to the neighborsNums vector

			if (GameBoard[i][j]) // alive
			{
				if (neighbors == 2 || neighbors == 3) // if the cell has 2 or 3 neighbors
				{
					sandbox[i].push_back(true); 
					++livingCells; 
				}
				else
				{
					sandbox[i].push_back(false); 
				}
			}
			else // dead
			{
				if (neighbors == 3) // if the cell has 3 neighbors
				{
					sandbox[i].push_back(true); 
					++livingCells; 
				}
				else
				{
					sandbox[i].push_back(false);
				}
			}
		}
	}

	GameBoard.swap(sandbox); // update the gameboard
	++Generations; // increment the number of generations
	setStatusBar(); // update the status bar
	Main_DrawingPanel_Ptr->Refresh(); // refresh the drawing panel
	settings.Save(); // save the settings
}
//----------------------------RANDOMIZE OPTIONS-----------------------------------------------------------------------------------------------------
void MainWindow::OnRandomizeClicked(wxCommandEvent& event) 
{
	Randomize(time(NULL)); //  start randomize
	Main_DrawingPanel_Ptr->Refresh();
}
void MainWindow::Randomize(int seed) 
{
	srand(seed);  // random number generator

	// Loop through the grid
	for (int i = 0; i < GameBoard.size(); ++i)
	{
		for (int j = 0; j < GameBoard[0].size(); ++j)
		{
			// Use random number to determine if cell should be alive
			int ranoNum = rand();
			GameBoard[i][j] = (ranoNum % 2 == 0); // if the random number is even = cell is alive
		}
	}
}
void MainWindow::OnRandomizeSeed(wxCommandEvent& event)
{
	// Get num from the user
	long seed = wxGetNumberFromUser
	(
		"Enter the seed value:",  // Message displayed
		"Seed Value:",            // Label for the input field
		"Randomize Seed",         // Dialog title
		1,                        // Default value
		1,                        // Minimum value
		1000000                   // Maximum value
	);

	if (seed != -1) // cancel not clicked -> apply settings
	{
		Randomize(static_cast<int>(seed));  
		Main_DrawingPanel_Ptr->Refresh(); 
	}
}
//----------------------------GAMEPLAY OPTIONS------------------------------------------------------------------------------------------------------
void MainWindow::ToroidalGameplay(wxCommandEvent& event)
{
	settings.Toroidal = false;
	GetMenuBar()->FindItem(20001)->Check(true);
	GetMenuBar()->FindItem(20002)->Check(false);
}
void MainWindow::FiniteGameplay(wxCommandEvent& event)
{
	settings.Toroidal = true;
	GetMenuBar()->FindItem(20001)->Check(false);
	GetMenuBar()->FindItem(20002)->Check(true);
}

// 9/20/2023 FINAL SUBMISSION