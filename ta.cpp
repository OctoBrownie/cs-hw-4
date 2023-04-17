/*****************************************************************************************
 ** Program Filename: ta.cpp
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Implementation file for the TA class.
 ** Input: Depends on the function, but usually nothing.
 ** Output: Depends on the function, but generally gets information about the class.
 ****************************************************************************************/
#include<ncurses.h>
#include<string>
#include<random>

#include "ta.h"
#include "consts.h"

int TA::appeasedTurns = 0;

/*****************************************************************************************
 ** Function: getStatus
 ** Description: Gets the status string of the TAs.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
const std::string TA::getStatus() {
	if (appeasedTurns == 0) return TA_TEXT_NOT_APPEASED;

	return TA_TEXT_APPEASED_1 + std::to_string(appeasedTurns) + TA_TEXT_APPEASED_2;
}

/*****************************************************************************************
 ** Function: getAppeased
 ** Description: Gets whether the TAs are appeased or not.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
bool TA::getAppeased() { return appeasedTurns != 0; }

/*****************************************************************************************
 ** Function: decAppeased
 ** Description: Decrements the appeased turns of the TAs.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void TA::decAppeased() {
	if (appeasedTurns > 0) appeasedTurns--;
}

/*****************************************************************************************
 ** Function: setAppeased
 ** Description: Sets whether the TAs are appeased or not.
 ** Parameters: b (bool) - whether the TAs are appeased or not
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void TA::setAppeased(bool b) {
	if (b) appeasedTurns = 10;
	else appeasedTurns = 0;
}

/*****************************************************************************************
 ** Function: move
 ** Description: Gets the move of the TA. 
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int TA::move() const {
	switch(rand() % 4) {
		case 0: return MAZE_MOVE_LEFT;
		case 1: return MAZE_MOVE_RIGHT;
		case 2: return MAZE_MOVE_UP;
		case 3: return MAZE_MOVE_DOWN;
	}
	return MAZE_MOVE_UP;
}

/*****************************************************************************************
 ** Function: getDisplayChar
 ** Description: Gets the display character that represents the TA.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int TA::getDisplayChar() const {
	if (appeasedTurns) return 'T' | COLOR_PAIR(GAME_CPAIR_TA_DISABLED);
	return DISPLAY_CHAR_TA | COLOR_PAIR(GAME_CPAIR_TA_INSTRUCTOR);
}
