/*****************************************************************************************
 ** Program Filename: wall.cpp
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Implementation file for the Wall class.
 ** Input: Depends on the function, but usually nothing.
 ** Output: Depends on the function, but generally gets information about the class.
 ****************************************************************************************/
#include<ncurses.h>

#include "consts.h"
#include "wall.h"

/*****************************************************************************************
 ** Function: isOccupiable
 ** Description: Gets whether the current space is occupiable or not (always not).
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
bool Wall::isOccupiable() const { return false; }

/*****************************************************************************************
 ** Function: getDisplayChar
 ** Description: Gets the display character of the wall.
 ** Parameters: inFog (bool) - whether the space is shrouded in fog or not
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int Wall::getDisplayChar(bool inFog) const {
	return DISPLAY_CHAR_WALL | (inFog ? COLOR_PAIR(GAME_CPAIR_WALL_FOG) : 0);
}