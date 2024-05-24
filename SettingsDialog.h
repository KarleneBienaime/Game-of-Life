#pragma once
#include <wx/wx.h>
#include "Settings.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"
#include "wx/colordlg.h"

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

class SettingsDialog : public wxDialog
{
public:
	SettingsDialog(wxWindow* parent, GameSettings* drawSettings); // constructor

private: 

	GameSettings* drawSettings;

	wxSpinCtrl* gridSizeSpinner;
	wxColourPickerCtrl* livingCellColorPicker;
	wxColourPickerCtrl* deadCellColorPicker;

	void OnOK(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();

};
#endif // SETTINGSDIALOG_H
