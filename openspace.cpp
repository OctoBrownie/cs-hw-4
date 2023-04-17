/*****************************************************************************************
 ** Program Filename: openspace.cpp
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Implementation file for the OpenSpace class.
 ** Input: Depends on the function, but usually nothing.
 ** Output: Depends on the function, but it generally gives out information about the 
 **			class.
 ****************************************************************************************/
#include<ncurses.h>

#include "consts.h"
#include "openspace.h"

/*****************************************************************************************
 ** Function: OpenSpace (constructor)
 ** Description: Makes a new empty OpenSpace.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
OpenSpace::OpenSpace() : hasSkill(false) {}

/*****************************************************************************************
 ** Function: isOccupiable
 ** Description: Checks whether this space is occupiable (it always is).
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
bool OpenSpace::isOccupiable() const { return true; }

/*****************************************************************************************
 ** Function: getDisplayChar
 ** Description: Gets the character to display that represents the open space.
 ** Parameters: inFog (bool) - whether the space is shrouded in fog or not
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int OpenSpace::getDisplayChar(bool inFog) const {
	if (hasSkill && !inFog) return DISPLAY_CHAR_SKILL | COLOR_PAIR(GAME_CPAIR_SKILL);
	return DISPLAY_CHAR_EMPTY;
}

/*****************************************************************************************
 ** Function: getHasSkill
 ** Description: Checks whether this space has a skill in it.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
bool OpenSpace::getHasSkill() const { return hasSkill; }

/*****************************************************************************************
 ** Function: setHasSkills
 ** Description: Sets whether this space has a skill or not.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void OpenSpace::setHasSkill(bool b) { hasSkill = b; }