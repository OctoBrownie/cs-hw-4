/*****************************************************************************************
 ** Program Filename: intrepidstudent.cpp
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Implementation file for the IntrepidStudent class. 
 ** Input: Depends on the function, but generally nothing.
 ** Output: Depends on the function, but it generally gives out information about the 
 **			class. 
 ****************************************************************************************/
#include<ncurses.h>

#include "consts.h"
#include "intrepidstudent.h"
#include "ta.h"

/*****************************************************************************************
 ** Function: IntrepidStudent (constructor)
 ** Description: Makes a new IntrepidStudent object.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
IntrepidStudent::IntrepidStudent() : numSkills(0) {}

/*****************************************************************************************
 ** Function: getNumSkills
 ** Description: Returns the number of skills the player has right now.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int IntrepidStudent::getNumSkills() const { return numSkills; }

/*****************************************************************************************
 ** Function: move
 ** Description: Returns no move, since there was no input.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int IntrepidStudent::move() const { return MAZE_MOVE_NONE; }

/*****************************************************************************************
 ** Function: move
 ** Description: Returns the move the user wants to make. 
 ** Parameters: inputChar (int) - the character the user inputted
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int IntrepidStudent::move(int inputChar) const {
	switch(inputChar) {
		case 'w':
		case KEY_UP:
			return MAZE_MOVE_UP;
		case 'a':
		case KEY_LEFT:
			return MAZE_MOVE_LEFT;
		case 's':
		case KEY_DOWN:
			return MAZE_MOVE_DOWN;
		case 'd':
		case KEY_RIGHT:
			return MAZE_MOVE_RIGHT;
		default:
			// for using a skill, space, or enter
			return MAZE_MOVE_NONE;
	}
}

/*****************************************************************************************
 ** Function: getDisplayChar
 ** Description: Gets the character to display that represents an IntrepidStudent (the  
 **				 char itself and its color).
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int IntrepidStudent::getDisplayChar() const {
	return DISPLAY_CHAR_STUDENT | COLOR_PAIR(GAME_CPAIR_PLAYER);
}

/*****************************************************************************************
 ** Function: addSkill
 ** Description: Adds a skill to the current IntrepidStudent object.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void IntrepidStudent::addSkill() { numSkills++; }

/*****************************************************************************************
 ** Function: useSkill
 ** Description: Uses a skill to appease all of the TAs.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: All of the TAs are appeased and the counter resets.
 ****************************************************************************************/
void IntrepidStudent::useSkill() {
	if (numSkills == 0) return;

	numSkills--;
	TA::setAppeased(true);
}