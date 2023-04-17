/*****************************************************************************************
 ** Program Filename: optionspanel.cpp
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Implementation file for the OptionsPanel class.
 ** Input: Depends on the function, but usually nothing.
 ** Output: Depends on the function, but it draws things to screen or changes things about
 **			the panel.
 ****************************************************************************************/
#include<fstream>
#include<ncurses.h>

#include "optionspanel.h"
#include "consts.h"
#include "ncursesutils.h"
#include "startpanel.h"

/*****************************************************************************************
 ** Function: loadFromFile
 ** Description: Loads options from file into the output parameters.
 ** Parameters: diff (int&) - output parameter for maze difficulty
 **				fog (bool&) - output parameter for whether to use fog of war or not
 **				ai (bool&) - output parameter for whether to use the AI or not
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void OptionsPanel::loadFromFile(int& diff, bool& fog, bool& ai) {
	std::ifstream file;
	file.open(OPTION_FILE_NAME);
	if (!file.good()) {
		diff = OPTION_DIFF_DEFAULT;
		fog = OPTION_FOG_DEFAULT;
		ai = OPTION_AI_DEFAULT;
		return;
	}
	file >> diff >> fog >> ai;
	file.close();
}

/*****************************************************************************************
 ** Function: saveToFile
 ** Description: Saves all settings to file.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void OptionsPanel::saveToFile() {
	std::ofstream file;
	file.open(OPTION_FILE_NAME, std::ios::trunc);
	if (!file.good()) return;
	
	file << difficulty << ' ' << fogOfWar << ' ' << useAI;
	
	file.close();
}

/*****************************************************************************************
 ** Function: OptionsPanel (constructor)
 ** Description: Makes a new OptionsPanel.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
OptionsPanel::OptionsPanel() : currOption(OPTION_BUTTON_BACK) {}

/*****************************************************************************************
 ** Function: ~OptionsPanel (destructor)
 ** Description: Destroys the current OptionsPanel.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
OptionsPanel::~OptionsPanel() {}

/*****************************************************************************************
 ** Function: init
 ** Description: Initializes the OptionsPanel screen and also reads settings from file.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void OptionsPanel::init() {
	loadFromFile(difficulty, fogOfWar, useAI);

	resizeHandler();	// screen size check, also draw the screen 
}

/*****************************************************************************************
 ** Function: loop
 ** Description: This is the input loop for OptionsPanel.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: Only returns when the user wants to move to another screen, specified
 **					 in the return value.
 ****************************************************************************************/
Panel* OptionsPanel::loop() {
	Panel* nextPanel = nullptr;
	while (true) {
		int chr = getch();
		switch(chr) {
			case 'q':
				// quit
				return nullptr;
			case 'w': case KEY_UP:
				redrawButton((currOption + 3) % 4);
				break;
			case 's': case KEY_DOWN:
				redrawButton((currOption + 1) % 4);
				break;
			case 'a': case KEY_LEFT:
				handleLeftRightInput(true);
				break;
			case 'd': case KEY_RIGHT:
				handleLeftRightInput(false);
				break;
			case ' ':
			case '\n': case '\r': case KEY_ENTER:
				if (handleEnterInput(nextPanel)) return nextPanel;
		}
	}
}

/*****************************************************************************************
 ** Function: getSpace
 ** Description: Gets the number of rows that should be between elements.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int OptionsPanel::getSpace() {
	// 5 spaces total (between difficulty, fog, ai, and back)
	// fog and ai take up 3 lines each
	// there are two spaces between options and back button
	return (LINES - 8)/6;
}

/*****************************************************************************************
 ** Function: draw
 ** Description: Draws the entire OptionsPanel.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void OptionsPanel::draw() {
	drawDescription(OPTION_BUTTON_DIFF);
	drawButton(OPTION_BUTTON_DIFF);

	drawDescription(OPTION_BUTTON_FOG);
	drawButton(OPTION_BUTTON_FOG);

	drawDescription(OPTION_BUTTON_AI);
	drawButton(OPTION_BUTTON_AI);

	// drawDescription(OPTION_BUTTON_BACK);
	drawButton(OPTION_BUTTON_BACK);
}

/*****************************************************************************************
 ** Function: drawDescription
 ** Description: Draws the description for a specific button.
 ** Parameters: button (int) - the button of the description to draw
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void OptionsPanel::drawDescription(int button) {
	// draws the static parts of each option (descriptions and boxes, all left aligned)
	switch(button) {
		case OPTION_BUTTON_DIFF:
			mvaddstr(getSpace(), OPTION_LEFT_MARGIN, OPTION_DIFF_DESC);
			addch(ACS_LARROW);
			break;
		case OPTION_BUTTON_FOG:
			mvaddstr(2*getSpace()+2, OPTION_LEFT_MARGIN, OPTION_FOG_DESC);
			Util::drawBox(2*getSpace()+1, OPTION_LEFT_MARGIN + sizeof(OPTION_FOG_DESC), 3, 3);
			break;
		case OPTION_BUTTON_AI:
			mvaddstr(3*getSpace()+5, OPTION_LEFT_MARGIN, OPTION_AI_DESC);
			Util::drawBox(3*getSpace()+4, OPTION_LEFT_MARGIN + sizeof(OPTION_AI_DESC), 3, 3);
			break;
	}
}

/*****************************************************************************************
 ** Function: drawButton
 ** Description: Draws a specific button.
 ** Parameters: button (int) - the button to draw
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void OptionsPanel::drawButton(int button) {
	int attr = 0, currChar;
	if (button == currOption) attr = A_STANDOUT;

	// draws the dynamic parts of each option (buttons and arrows after, all left aligned)
	switch(button) {
		case OPTION_BUTTON_DIFF:
			// also draws the right arrow after the difficulty
			move(getSpace(), OPTION_LEFT_MARGIN + sizeof(OPTION_DIFF_DESC));
			clrtoeol();
			move(getSpace(), OPTION_LEFT_MARGIN + sizeof(OPTION_DIFF_DESC));
			switch(difficulty) {
				case OPTION_DIFF_EASY:
					Util::drawString(OPTION_DIFF_EASY_DESC, attr);
					break;
				case OPTION_DIFF_MED:
					Util::drawString(OPTION_DIFF_MED_DESC, attr);
					break;
				case OPTION_DIFF_HARD:
					Util::drawString(OPTION_DIFF_HARD_DESC, attr);
					break;
			}
			addch(ACS_RARROW);
			break;
		case OPTION_BUTTON_FOG:
			currChar = (fogOfWar ? OPTION_CHECK_ON : OPTION_CHECK_OFF) | attr;
			mvaddch(2*getSpace()+2, OPTION_LEFT_MARGIN+sizeof(OPTION_FOG_DESC)+1, currChar);
			break;
		case OPTION_BUTTON_AI:
			currChar = (useAI ? OPTION_CHECK_ON : OPTION_CHECK_OFF) | attr;
			mvaddch(3*getSpace()+5, OPTION_LEFT_MARGIN+sizeof(OPTION_AI_DESC)+1, currChar);
			break;
		case OPTION_BUTTON_BACK:
			Util::drawCentered(5*getSpace() + 7, OPTION_BACK_DESC, attr);
			break;
	}
}

/*****************************************************************************************
 ** Function: redrawButton
 ** Description: Redraws relevant parts of the screen when another button is chosen.
 ** Parameters: newOption (int) - the new button that the user navigated to
 ** Pre-Conditions: none.
 ** Post-Conditions: currOption is equal to newOption.
 ****************************************************************************************/
void OptionsPanel::redrawButton(int newOption) {
	int oldOption = currOption;
	currOption = newOption;

	if (currOption != oldOption) drawButton(oldOption);
	drawButton(currOption);
}

/*****************************************************************************************
 ** Function: handleLeftRightInput
 ** Description: Handles inputs that are either the user clicking left or right. 
 ** Parameters: left (bool) - whether the user went to the left or not 
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void OptionsPanel::handleLeftRightInput(bool left) {
	// only matters for difficulty settings
	if (currOption != OPTION_BUTTON_DIFF) return;
	
	if (left && difficulty != OPTION_DIFF_EASY) difficulty--;
	else if (!left && difficulty != OPTION_DIFF_HARD) difficulty++;

	saveToFile();
	redrawButton(currOption);
}

/*****************************************************************************************
 ** Function: handleEnterInput
 ** Description: Handles an enter input and returns the new panel if the user is 
 **				 navigating to another panel. 
 ** Parameters: nextPanel (Panel*&) - the new panel to show, if necessary 
 ** Pre-Conditions: none.
 ** Post-Conditions: If the function returns true, the user wants to navigate to another 
 **					 panel and nextPanel is filled accordingly. Otherwise, nextPanel
 **					 remains unchanged.
 ****************************************************************************************/
bool OptionsPanel::handleEnterInput(Panel*& nextPanel) {
	switch(currOption) {
		case OPTION_BUTTON_BACK:
			nextPanel = new StartPanel();
			return true;
		case OPTION_BUTTON_DIFF: 
			return false;
		case OPTION_BUTTON_FOG:
			fogOfWar = !fogOfWar;
			saveToFile();
			break;
		case OPTION_BUTTON_AI:
			useAI = !useAI;
			saveToFile();
			break;
	}

	redrawButton(currOption);
	return false;
}


