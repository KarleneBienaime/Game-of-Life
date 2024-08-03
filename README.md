# Game of Life

This is an implementation of Conway's Game of Life using C++ and wxWidgets.

## Features

- Interactive grid where users can toggle cells on and off
- Play, pause, and step through generations
- Randomize the grid or use a specific seed
- Adjustable grid size
- Finite and toroidal boundary options
- Import and export game states
- Show/hide grid lines and neighbor counts
- Customizable settings

## Dependencies

- C++
- wxWidgets library

## How to Run

1. Ensure you have wxWidgets installed on your system.
2. Compile the source code with your C++ compiler, linking against wxWidgets.
3. Run the compiled executable.

## Controls

- Play: Start the simulation
- Pause: Pause the simulation
- Next: Advance one generation
- Trash: Clear the grid

## Menu Options

### File
- New: Start a new game
- Open: Load a saved game state
- Save/Save As: Save the current game state
- Reset Settings: Restore default settings
- Import: Import a game board from a .cells file
- Exit: Close the application

### Options
- Randomize: Randomly populate the grid
- Randomize with seed: Populate the grid using a specific seed
- Finite/Toroidal: Toggle between finite and toroidal boundary conditions

### View
- Show Neighbors Count: Display the number of live neighbors for each cell
- Show Grid: Toggle the display of grid lines
- Show 10x10 Grid: Toggle the display of a larger grid overlay
- Show HUD: Toggle the display of the heads-up display

## Game Rules

The Game of Life follows these rules:

1. Any live cell with fewer than two live neighbors dies (underpopulation).
2. Any live cell with two or three live neighbors lives on to the next generation.
3. Any live cell with more than three live neighbors dies (overpopulation).
4. Any dead cell with exactly three live neighbors becomes a live cell (reproduction).
