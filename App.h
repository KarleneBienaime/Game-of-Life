#pragma once // The #pragma once directive is used to avoid multiple inclusions of the same file.
#include "MainWindow.h"
#include "wx/wx.h"

class App : public wxApp //wxApp is a class that is part of the wxWidgets library.
{
private:
	MainWindow* App_MainW_Ptr = nullptr; //MainWindow pointer

public:
	App(); // Create a constructor for the App class.
	~App(); // Create a destructor for the App class.

	virtual bool OnInit(); // OnInit() is a virtual method inherited from wxApp.
};

