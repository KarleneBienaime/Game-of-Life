#include "App.h"

wxIMPLEMENT_APP(App); // Instance of the App class and call it App.

App::App() 
{

}
App::~App() 
{
	delete App_MainW_Ptr; // Delete the MainWindow pointer
}
bool App::OnInit() 
{

	GameSettings drawSettings; // Settings object
	drawSettings.Load(); // Load the settings
	MainWindow* AC_MainW_Ptr = new MainWindow(); // Instantiate the MainWindow pointer
	AC_MainW_Ptr->Show(); // Show() is a method for wxframe objects that displays the window.
	return true;
}