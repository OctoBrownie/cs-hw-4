/*****************************************************************************************
 ** Program Filename: ncursesutils.h
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Header file for some utilities for ncurses.
 ** Input: Depends on the function, but usually the row to draw stuff, the text to draw, 
 **		   and the attributes on the text to draw.
 ** Output: Depends on the function, but it usually draws stuff to screen.
 ****************************************************************************************/
#ifndef __ncurses_utils_h__
#define __ncurses_utils_h__

#include<ncurses.h>
#include<string>

namespace Util {
	void drawCentered(int row, std::string str, int attr);
	void drawCentered(WINDOW* win, int row, std::string str, int attr);
	
	void drawString(std::string str, int attr);
	void drawString(WINDOW* win, std::string str, int attr);

	// inside of box is within the border (startRow + 1, startCol + 1) up to 
	// (startRow + rows - 1, startCol + cols - 1)
	void drawBox(int startRow, int startCol, int rows, int cols);
}

#endif 