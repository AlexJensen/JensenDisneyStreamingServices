#ifndef DISNEY_INTERPRETER_ITEM_TYPES
#define DISNEY_INTERPRETER_ITEM_TYPES

/*
* DisneyMenuItemTypes.H
* Defines information for multiple different types of menu objects. Functions similar to a primitive CSS file to store various *magic* numbers for easy tweaking of values during testing and development
* Feel free to add as many different types as you need for various menu types.
*/

/// <summary>
/// Controls for how the main menu should draw.
/// </summary>
struct mainMenuButtonGrid
{
	// Defines what a button should look like, where to draw the grid, and how much space should be between each element.
	const float width = 200, height = 112, xstart = 20, ystart = 20, xgap = 10, ygap = 10, ygapheader = 50, xgapselect = 10, ygapselect = 10, xBorder = 2, yBorder = 2;

	// Keeps track of current pointer position
	float xPos = xstart, yPos = ystart;

	// Dynamic reallocation of borders depends on last input set
	float xBorderL = xBorder, xBorderR = xBorder, yBorderU = yBorder, yBorderD = yBorder;

	// Determines the currently selected item in the grid
	int menuX = 0, menuY = 0;
};

#endif