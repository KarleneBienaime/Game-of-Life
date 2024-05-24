#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

#include <wx/wx.h>
#include <fstream>

struct GameSettings
{
    unsigned int livingCellRed = 128; 
    unsigned int livingCellGreen = 128;
    unsigned int livingCellBlue = 128;
    unsigned int livingCellAlpha = 255;

    unsigned int deadCellRed = 255;
    unsigned int deadCellGreen = 255;
    unsigned int deadCellBlue = 255;
    unsigned int deadCellAlpha = 255;

    int gridSize = 15;
    int interval = 50;

    bool showNeighborCount = true;
    bool Toroidal = false;
    bool Finite = true;
    bool ShowGrid = true;
    bool ShowTenGrid = false;
    bool ShowHUD = true;

    wxColor Get_LivingCellColor() const 
    {
        return wxColor(livingCellRed, livingCellGreen, livingCellBlue, livingCellAlpha);
    }

    wxColor Get_DeadCellColor() const
    {
        return wxColor(deadCellRed, deadCellGreen, deadCellBlue, deadCellAlpha);
    }

    void Set_LivingCellColor(const wxColor& color) 
    {
        livingCellRed = color.Red();
        livingCellGreen = color.Green();
        livingCellBlue = color.Blue();
        livingCellAlpha = color.Alpha();
    }

    void Set_DeadCellColor(const wxColor& color) 
    {
        deadCellRed = color.Red();
        deadCellGreen = color.Green();
        deadCellBlue = color.Blue();
        deadCellAlpha = color.Alpha();
    }
    void Load() 
    {
        std::ifstream file("settings.bin", std::ios::binary | std::ios::in); // open the file
        if (file.is_open()) //
        {
            file.read((char*)this, sizeof(GameSettings)); // read char pointer, this, and the size of the game settings
            file.close(); 
        }
    }
    void Save() 
    {
        std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
        if (file.is_open()) 
        {
            file.write((char*)this, sizeof(GameSettings));
            file.close(); // MAKE SURETO CLOSE THE FILE!!!
        }
    }
    void Reset()
    {
		livingCellRed = 128;
		livingCellGreen = 128;
		livingCellBlue = 128;
		livingCellAlpha = 255;

		deadCellRed = 255;
		deadCellGreen = 255;
		deadCellBlue = 255;
		deadCellAlpha = 255;

		gridSize = 15;
		interval = 50;

		showNeighborCount = true;
		Toroidal = false;
		Finite = true;
	}
};
#endif // SETTINGS_H

