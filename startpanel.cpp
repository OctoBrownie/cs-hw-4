/*****************************************************************************************
 ** Program Filename: startpanel.cpp
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Implementation file for the StartPanel class.
 ** Input: Depends on the function, but usually nothing.
 ** Output: Depends on the function, but generally changes some things about the screen or
 **			updates internal variables.
 ****************************************************************************************/
#include<ncurses.h>
#include<string>
#include<iostream>

#include "consts.h"
#include "ncursesutils.h"

#include "panel.h"
#include "startpanel.h"
#include "optionspanel.h"
#include "game.h"

/*****************************************************************************************
 ** Function: StartPanel (constructor)
 ** Description: Makes a new StartPanel with a default square maze of size 10.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
StartPanel::StartPanel() : StartPanel(10) {}

/*****************************************************************************************
 ** Function: StartPanel (constructor)
 ** Description: Makes a new StartPanel with a default square maze of the given size.
 ** Parameters: mazeSize (int) - the size of the square maze to default to
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
StartPanel::StartPanel(int mazeSize) : mazeWidth(mazeSize), mazeHeight(mazeSize), 
	isSquare(true), currButton(START_BUTTON_START) {}

/*****************************************************************************************
 ** Function: StartPanel (constructor)
 ** Description: Makes a new StartPanel with a default maze of the given size.
 ** Parameters: mazeWidth (int) - the width of the maze to default to
 **				mazeHeight (int) - the height of the maze to default to
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
StartPanel::StartPanel(int mazeWidth, int mazeHeight) : mazeWidth(mazeWidth), 
	mazeHeight(mazeHeight), isSquare(mazeWidth == mazeHeight), 
	currButton(START_BUTTON_START) {}

/*****************************************************************************************
 ** Function: init
 ** Description: Initializes the screen and the relevant color pairs.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void StartPanel::init() {
	Panel::init();	// init colors
	init_pair(START_CPAIR_TITLE, COLOR_BLUE, COLOR_BLACK);
	init_pair(START_CPAIR_DISABLED, COLOR_CYAN, COLOR_BLACK);

	resizeHandler();	// screen size check, also draw the screen 
}

/*****************************************************************************************
 ** Function: loop
 ** Description: Input loop for the StartPanel.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: Only returns when the user wants to move to another screen, specified
 **					 in the return value.
 ****************************************************************************************/
Panel* StartPanel::loop() {
	while(true) {
		int chr = getch();		// automatically calls refresh (don't need to manually)

		if (windowDisabled && chr != KEY_RESIZE) continue;
		switch (chr) {
			case KEY_RESIZE: 
				resizeHandler();
				break;
			case 'w': case KEY_UP: 
				selectNextButton(true);
				break;
			case 's': case KEY_DOWN: 
				selectNextButton(false);
				break;
			case 'a': case KEY_LEFT:
				changeMazeSize(false);
				break;
			case 'd': case KEY_RIGHT:
				changeMazeSize(true);
				break;
			case ' ':
			case '\n': case '\r': case KEY_ENTER: {
				Panel* p = nullptr;
				if (handleEnter(p)) return p;
				break;
			}
		}
	}
	return nullptr;
}

/*****************************************************************************************
 ** Function: selectNextButton
 ** Description: Selects the next button (goes either up or down).
 ** Parameters: goUp (bool) - whether to go up or down 
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void StartPanel::selectNextButton(bool goUp) {
	int prevButton = currButton;
	currButton = (currButton + (goUp ? 5 : 1)) % 6;
	
	drawButton(prevButton);
	drawButton(currButton);
}

/*****************************************************************************************
 ** Function: changeMazeSize
 ** Description: Handles when the user goes left or right. Attempts to change the maze 
 **				 size if the user is on the right buttons. If it's on height and it's 
 **				 disabled (maze is square right now), will deselect square maze and change 
 **				 it anyway.
 ** Parameters: increment (bool) - whether to increment or decrement if possible
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void StartPanel::changeMazeSize(bool increment) {
	if (currButton != START_BUTTON_WIDTH && currButton != START_BUTTON_HEIGHT)
		return;

	int* len = currButton == START_BUTTON_WIDTH ? &mazeWidth : &mazeHeight;
	if (increment) {
		if (*len == START_MAX_MAZE_SIZE) return;
		(*len)++;
	}
	else {
		if (*len == START_MIN_MAZE_SIZE) return;
		(*len)--;
	}
	
	if (isSquare) {
		if (currButton == START_BUTTON_HEIGHT) {
			isSquare = false;
			drawButton(START_BUTTON_SQUARE);
		}
		else {
			mazeHeight = mazeWidth;
			drawButton(START_BUTTON_HEIGHT);
		}
	}
	drawButton(currButton);
}

/*****************************************************************************************
 ** Function: handleEnter
 ** Description: Handles when the user clicks enter.
 ** Parameters: nextPanel (Panel*&) - output parameter that gives the next panel to 
 **					nagivate to, if necessary 
 ** Pre-Conditions: none.
 ** Post-Conditions: Only returns true when the user wants to move to another screen, and
 **					 will populate nextPanel correctly
 ****************************************************************************************/
bool StartPanel::handleEnter(Panel*& nextPanel) {
	switch(currButton) {
		case START_BUTTON_SQUARE:
			isSquare = !isSquare;
			drawButton(START_BUTTON_SQUARE);
			drawButton(START_BUTTON_HEIGHT);
			return false;
		case START_BUTTON_WIDTH:
		case START_BUTTON_HEIGHT:
			return false;
		case START_BUTTON_START:
			nextPanel = new Game(mazeWidth, mazeHeight);
			return true;
		case START_BUTTON_OPTIONS:
			nextPanel = new OptionsPanel();
			return true;
		case START_BUTTON_QUIT:
			nextPanel = nullptr;
			return true;
	}
}

/*****************************************************************************************
 ** Function: calculateSpace
 ** Description: Gets the number of rows between all of the vertical elements.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int StartPanel::calculateSpace() {
	// size of "glue" between the title, start, options, and exit changes size with height
	// 2 before and after title, 1 for all other spaces 
	// remember that the square checkbox takes up 3 spaces
	return (LINES - 9) / 10;
}

/*****************************************************************************************
 ** Function: drawButton
 ** Description: Draws the specified button.
 ** Parameters: button (int) - the button to draw
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void StartPanel::drawButton(int button) {
	int attr = currButton == button ? A_STANDOUT : 0;
	int space = calculateSpace();

	switch(button) {
		case START_BUTTON_SQUARE:
			drawIsSquare(4*space + 2, attr);
			break;
		case START_BUTTON_WIDTH:
			drawField(5*space + 4, true, attr);
			break;
		case START_BUTTON_HEIGHT:
			drawField(6*space + 5, false, attr);
			break;
		case START_BUTTON_START:
			Util::drawCentered(7*space + 6, START_TEXT_START, attr);
			break;
		case START_BUTTON_OPTIONS:
			Util::drawCentered(8*space + 7, START_TEXT_OPTIONS, attr);
			break;
		case START_BUTTON_QUIT:
			Util::drawCentered(9*space + 8, START_TEXT_QUIT, attr);
			break;
	}
}

/*****************************************************************************************
 ** Function: drawField
 ** Description: Draws the specified field (either the maze width or height fields).
 ** Parameters: row (int) - the row to draw the field
 **				isWidth (bool) - whether to draw width or height
 **				attr (int) - text attributes to add to the selectable portion (the actual
 **					number of rows/cols)
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void StartPanel::drawField(int row, bool isWidth, int attr) {
	move(row, 0);
	clrtoeol();

	int* len;
	const char* text;
	int textAttr = 0;
	if (isWidth) {
		len = &mazeWidth;
		text = START_TEXT_WIDTH;
	}
	else {
		len = &mazeHeight;
		text = START_TEXT_HEIGHT;
		textAttr = isSquare ? COLOR_PAIR(START_CPAIR_DISABLED) : 0;
		attr |= textAttr;
	}

	// for arrows
	move(row, (COLS - (text + std::to_string(*len)).length() - 4)/2);
	Util::drawString(text, textAttr);
	addch(ACS_LARROW | textAttr);
	Util::drawString(' ' + std::to_string(*len) + ' ', attr);
	addch(ACS_RARROW | textAttr);
}

/*****************************************************************************************
 ** Function: drawIsSquare
 ** Description: Draws the isSquare checkbox.
 ** Parameters: row (int) - the center row for the checkbox (draws on three rows)
 **				attr (int) - text attributes to add to the selectable portion (the actual
 **					checkbox part)
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void StartPanel::drawIsSquare(int row, int attr) {
	// clear the THREE rows
	for (int i = row-1; i <= row+1; i++) {
		move(row - 1, 0);
		clrtoeol();
	}

	// draw description (total space is START_TEXT_SQUARE plus the 3x3 box)
	int col = (COLS - sizeof(START_TEXT_SQUARE) - 2)/2;
	mvaddstr(row, col, START_TEXT_SQUARE);

	// draw checkbox (box and center char)
	col += sizeof(START_TEXT_SQUARE) - 1;	// offset by just the text
	Util::drawBox(row - 1, col, 3, 3);
	mvaddch(row, col + 1, (isSquare ? OPTION_CHECK_ON : OPTION_CHECK_OFF) | attr);
}

/*****************************************************************************************
 ** Function: draw
 ** Description: Draws the entire StartPanel.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void StartPanel::draw() {
	// draw title
	Util::drawCentered(2*calculateSpace(), START_TEXT_TITLE, 
					   A_BOLD | COLOR_PAIR(START_CPAIR_TITLE));

	// draw buttons
	drawButton(START_BUTTON_SQUARE);
	drawButton(START_BUTTON_WIDTH);
	drawButton(START_BUTTON_HEIGHT);
	drawButton(START_BUTTON_START);
	drawButton(START_BUTTON_OPTIONS);
	drawButton(START_BUTTON_QUIT);
}
