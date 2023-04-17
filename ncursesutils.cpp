/*****************************************************************************************
 ** Program Filename: ncursesutils.cpp
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Implementation file for some utilities for ncurses.
 ** Input: Depends on the function, but usually the row to draw stuff, the text to draw, 
 **		   and the attributes on the text to draw.
 ** Output: Depends on the function, but it usually draws stuff to screen.
 ****************************************************************************************/
#include "ncursesutils.h"

/*****************************************************************************************
 ** Function: drawCentered
 ** Description: Centers and draws the given string.
 ** Parameters: row (int) - the row to draw in 
 **				str (string) - text to draw
 **				attr (int) - text attributes
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Util::drawCentered(int row, std::string str, int attr) {
	wmove(stdscr, row, (COLS - str.length())/2);
	drawString(stdscr, str, attr);
}

/*****************************************************************************************
 ** Function: drawCentered
 ** Description: Centers and draws the given string.
 ** Parameters: win (WINDOW*) - the window to draw to
 **				row (int) - the row to draw in 
 **				str (string) - text to draw
 **				attr (int) - text attributes
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Util::drawCentered(WINDOW* win, int row, std::string str, int attr) {
	int rows, cols;
	getmaxyx(win, rows, cols);

	wmove(win, row, (cols - str.length())/2);
	drawString(win, str, attr);
}

/*****************************************************************************************
 ** Function: drawString
 ** Description: Draws the given string.
 ** Parameters: str (string) - text to draw
 **				attr (int) - text attributes
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Util::drawString(std::string str, int attr) { drawString(stdscr, str, attr); }

/*****************************************************************************************
 ** Function: drawString
 ** Description: Draws the given string.
 ** Parameters: win (WINDOW*) - the window to draw to
 **				str (string) - text to draw
 **				attr (int) - text attributes
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Util::drawString(WINDOW* win, std::string str, int attr) {
	const char* nameChars = str.c_str();

	for (int i = 0; i < str.length(); i++) {
		waddch(win, nameChars[i] | attr);
	}
}

/*****************************************************************************************
 ** Function: drawBox
 ** Description: Draws a box on screen.
 ** Parameters: startRow (int) - the row to start on
 **				startCol (int) - the column to start on
 **				rows (int) - the total number of rows of the box (including border)
 **				cols (int) - the total number of columns of the box (including border)
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Util::drawBox(int startRow, int startCol, int rows, int cols) {
	// top row
	mvaddch(startRow, startCol, ACS_ULCORNER);
	for (int i = 0; i < rows - 2; i++) addch(ACS_HLINE);
	addch(ACS_URCORNER);

	// vertical lines
	for (int i = 0; i < cols - 2; i++) {
		mvaddch(startRow + i + 1, startCol, ACS_VLINE);
		mvaddch(startRow + i + 1, startCol + cols - 1, ACS_VLINE);
	}

	// bottom row
	mvaddch(startRow + rows - 1, startCol, ACS_LLCORNER);
	for (int i = 0; i < rows - 2; i++) addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}
