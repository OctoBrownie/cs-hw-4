/*****************************************************************************************
 ** Program Filename: instructor.cpp
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Implementation file for the Instructor class. 
 ** Input: For all functions here, none.
 ** Output: Depends on the function, but it generally gives out information about the 
 **			class. 
 ****************************************************************************************/
#include<ncurses.h>

#include "consts.h"
#include "instructor.h"

/*****************************************************************************************
 ** Function: move
 ** Description: Since the instructor doesn't move... it just returns no move.
 ** Parameters: none.
 ** Pre-Conditions: none. 
 ** Post-Conditions: none.
 ****************************************************************************************/
int Instructor::move() const { return MAZE_MOVE_NONE; }

/*****************************************************************************************
 ** Function: getDisplayChar
 ** Description: Gets the character to display that represents an Instructor (the char 
 **				 itself and its color).
 ** Parameters: none.
 ** Pre-Conditions: Game colors have already been initialized. 
 ** Post-Conditions: none.
 ****************************************************************************************/
int Instructor::getDisplayChar() const {
	return DISPLAY_CHAR_INSTRUCTOR | COLOR_PAIR(GAME_CPAIR_TA_INSTRUCTOR);
}