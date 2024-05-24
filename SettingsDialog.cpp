#include "SettingsDialog.h"

wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
EVT_BUTTON(wxID_OK, SettingsDialog::OnOK) // when the ok button is clicked
EVT_BUTTON(wxID_CANCEL, SettingsDialog::OnCancel) // when the cancel button is clicked
wxEND_EVENT_TABLE()

SettingsDialog::SettingsDialog(wxWindow * parent, GameSettings * drawSettings) : wxDialog(parent, wxID_ANY, "Settings", wxDefaultPosition, wxDefaultSize)
{
    this->drawSettings = drawSettings; // set settings to the settings passed in
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL); // vertical box

//-------------------------------GRIDSIZE-----------------------------------------------------------------

    wxBoxSizer* gridSizeSizer = new wxBoxSizer(wxHORIZONTAL);                                                // horizontal box
    wxStaticText* gridSizeLabel = new wxStaticText(this, wxID_ANY, "Grid Size:");                           // label
    gridSizeSpinner = new wxSpinCtrl(this, wxID_ANY, wxString::Format("%d", drawSettings->gridSize));      // spinner
    gridSizeSizer->Add(gridSizeLabel, 0, wxALIGN_CENTER_VERTICAL);                                        // add label to the sizer
    gridSizeSizer->Add(gridSizeSpinner, 0, wxALIGN_CENTER_VERTICAL);                                     // add spinner to the sizer
    mainSizer->Add(gridSizeSizer, 0, wxALL | wxEXPAND, 10);                                             // add the sizer to the main sizer

//-------------------------------LIVING CELLS----------------------------------------------------------

    wxBoxSizer* livingCellColorSizer = new wxBoxSizer(wxHORIZONTAL);                               // horizontal box
    wxStaticText* livingCellColorLabel = new wxStaticText(this, wxID_ANY, "Living Cell Color:");  // label
    livingCellColorPicker = new wxColourPickerCtrl(this, wxID_ANY, *wxBLACK);                    // color picker *IMPORTANT*
    livingCellColorSizer->Add(livingCellColorLabel, 0, wxALIGN_CENTER_VERTICAL);                // add label to the sizer
    livingCellColorSizer->Add(livingCellColorPicker, 0, wxALIGN_CENTER_VERTICAL);              // add color picker to the sizer
    mainSizer->Add(livingCellColorSizer, 0, wxALL | wxEXPAND, 10);                            // add the sizer to the main sizer

//-------------------------------DEAD CELLS----------------------------------------------------------------------

    wxBoxSizer* deadCellColorSizer = new wxBoxSizer(wxHORIZONTAL);                              // horizontal box
    wxStaticText* deadCellColorLabel = new wxStaticText(this, wxID_ANY, "Dead Cell Color:");   // label
    deadCellColorPicker = new wxColourPickerCtrl(this, wxID_ANY, *wxWHITE);					  // color picker *IMPORTANT*
    deadCellColorSizer->Add(deadCellColorLabel, 0, wxALIGN_CENTER_VERTICAL);				 // add label to the sizer
    deadCellColorSizer->Add(deadCellColorPicker, 0, wxALIGN_CENTER_VERTICAL);			    // add color picker to the sizer
    mainSizer->Add(deadCellColorSizer, 0, wxALL | wxEXPAND, 10);						   // add the sizer to the main sizer

//-----------------------------------------------------------------------------------------------------

    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL); // create the buttons
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER); // add the buttons to the main sizer

    SetSizerAndFit(mainSizer); // set the sizer and fit the dialog around it
    gridSizeSpinner->SetValue(drawSettings->gridSize); // spinner value = grid size
}
void SettingsDialog::OnOK(wxCommandEvent & event)
{
    // Update the settings object with values from controls
    drawSettings->gridSize = gridSizeSpinner->GetValue();

    wxColor livingColor = livingCellColorPicker->GetColour(); //
    drawSettings->Set_LivingCellColor(livingColor);
    wxColor deadColor = deadCellColorPicker->GetColour();
    drawSettings->Set_DeadCellColor(deadColor);

    drawSettings->Save();
    EndModal(wxID_OK);
}
void SettingsDialog::OnCancel(wxCommandEvent & event)
{
    EndModal(wxID_CANCEL); // close
}