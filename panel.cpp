/*****************************************************************************************
 ** Program Filename: panel.cpp
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Implementation file for the Panel class.
 ** Input: Depends on the function, but usually nothing.
 ** Output: Depends on the function, but generally changes some things about the screen.
 ****************************************************************************************/
#include<ncurses.h>
#include<iostream>

#include "consts.h"
#include "panel.h"

/*****************************************************************************************
 ** Function: ~Panel (destructor)
 ** Description: Deletes the current Panel.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
Panel::~Panel() {}

/*****************************************************************************************
 ** Function: init
 ** Description: Initializes colors for all panels.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Panel::init() {
	if (can_change_color()) {
		init_color(COLOR_BLUE, PANEL_BLUE_R, PANEL_BLUE_G, PANEL_BLUE_B);
		init_color(COLOR_RED, PANEL_RED_R, PANEL_RED_G, PANEL_RED_B);
		init_color(COLOR_GREEN, PANEL_GREEN_R, PANEL_GREEN_G, PANEL_GREEN_B);
		init_color(COLOR_CYAN, PANEL_GREY_R, PANEL_GREY_G, PANEL_GREY_B);
	}
}

/*****************************************************************************************
 ** Function: resizeHandler
 ** Description: Clears the screen and checks that the screen size is large enough to 
 **				 play the game.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Panel::resizeHandler() {
	clear();

	if (COLS < MIN_COLS || LINES < MIN_LINES) {
		windowDisabled = true;
		move(0, 0);
		addstr(("ERROR: Your window is a bit too small... Please make it at least " + 
				std::to_string(MIN_COLS) + 'x' + std::to_string(MIN_LINES) + " (w x h).\n").c_str());
		addstr(("Current window size: " + std::to_string(COLS) + 'x' + std::to_string(LINES)).c_str());
		refresh();		// show standard screen on top
	}
	else {
		windowDisabled = false;
		draw();
	}
}