/*****************************************************************************************
 ** Program Filename: game.cpp
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Implementation file for the Game class. 
 ** Input: For the only function defined here, it's the map of the entire maze plus the 
 **		   people in it.
 ** Output: For the only function defined here, it's the move the AI wants to take. 
 ****************************************************************************************/
#include<ncurses.h>
#include<string>
#include<math.h>

#include "consts.h"
#include "ncursesutils.h"

#include "game.h"
#include "mazeperson.h"
#include "ta.h"

#include "startpanel.h"

/*****************************************************************************************
 ** Function: Game (constructor)
 ** Description: Makes a new Game object.
 ** Parameters: mazeSize (int) - size of the maze
 ** Pre-Conditions: none. 
 ** Post-Conditions: Will show an alert asking if the user wants a tutorial.
 ****************************************************************************************/
Game::Game(int mazeSize) : Game(mazeSize, mazeSize, true) {}

/*****************************************************************************************
 ** Function: Game (constructor)
 ** Description: Makes a new Game object.
 ** Parameters: mazeWidth (int) - the width of the maze (columns)
 **				mazeHeight (int) - the height of the maze (rows)
 ** Pre-Conditions: none. 
 ** Post-Conditions: Will show an alert asking if the user wants a tutorial.
 ****************************************************************************************/
Game::Game(int mazeWidth, int mazeHeight) : Game(mazeWidth, mazeHeight, true) {}

/*****************************************************************************************
 ** Function: Game (constructor)
 ** Description: Makes a new Game object.
 ** Parameters: mazeWidth (int) - the width of the maze (columns)
 **				mazeHeight (int) - the height of the maze (rows)
 **				tutorial (bool) - whether or not to show an alert asking if the user wants
 **					any tutorials
 ** Pre-Conditions: none. 
 ** Post-Conditions: none.
 ****************************************************************************************/
Game::Game(int mazeWidth, int mazeHeight, bool tutorial) : maze(Maze(mazeWidth, mazeHeight)), 
	hudWin(nullptr), mazeWin(nullptr), alertWin(nullptr), mazeWinRows(0), cameraCol(0), 
	cameraRow(0), hudRightCol(GAME_HUD_WIDTH_1), alertOption(0), alertNumOptions(0), 
	askTutorial(tutorial) {}

/*****************************************************************************************
 ** Function: ~Game (destructor)
 ** Description: Destroys the Game object.
 ** Parameters: none.
 ** Pre-Conditions: none. 
 ** Post-Conditions: none.
 ****************************************************************************************/
Game::~Game() {
	if (hudWin != nullptr) delwin(hudWin);
	if (mazeWin != nullptr) delwin(mazeWin);
	if (alertWin != nullptr) delwin(alertWin);
}

/*****************************************************************************************
 ** Function: init
 ** Description: initializes display-related things inside the Game class, and draws the 
 **				 initial screen. Will also check if the screen is big enough.  
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: All colors and color pairs are properly initialized, and a tutorial
 **					 alert is shown if asked for.  
 ****************************************************************************************/
void Game::init() {
	Panel::init();	// init colors

	// init color pairs
	init_pair(GAME_CPAIR_PLAYER, COLOR_GREEN, COLOR_BLACK);
	init_pair(GAME_CPAIR_TA_INSTRUCTOR, COLOR_RED, COLOR_BLACK);
	init_pair(GAME_CPAIR_TA_DISABLED, COLOR_WHITE, COLOR_BLACK);
	init_pair(GAME_CPAIR_SKILL, COLOR_BLUE, COLOR_BLACK);
	init_pair(GAME_CPAIR_WALL_FOG, COLOR_CYAN, COLOR_BLACK);
	
	if (askTutorial) {
		alertType = ALERT_TUTORIAL_ASK;
		initAlertWin();
	}
	
	resizeHandler();	// screen size check, also draw the screen 
}

/*****************************************************************************************
 ** Function: loop
 ** Description: Input loop for the program when showing a Game panel. Only returns when
 **				 there's a new panel to show (which is the return value), or nullptr if 
 **				 it's time to terminate the program.
 ** Parameters: none.
 ** Pre-Conditions: init() was already called.
 ** Post-Conditions: none.
 ****************************************************************************************/
Panel* Game::loop() {
	Panel* p = nullptr;
	int chr;
	while (true) {
		if (windowDisabled) chr = getch();
		else if (alertWin != nullptr) chr = wgetch(alertWin);
		else chr = wgetch(mazeWin);

		if (chr == KEY_RESIZE) resizeHandler();
		if (windowDisabled) continue;

		if (alertWin != nullptr) {
			if (handleInputAlert(chr, p)) break;
		}
		else {
			if (handleInputMaze(chr, p)) break;
		}
	}
	return p;
}

/*****************************************************************************************
 ** Function: resizeHandler
 ** Description: Called when the window has been resized. Updates some internal variables
 **				 and calls Panel::resizeHandler(). 
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: mazeWinRows is updated.
 ****************************************************************************************/
void Game::resizeHandler() {
	mazeWinRows = LINES - GAME_HUD_HEIGHT;

	Panel::resizeHandler();
}

/*****************************************************************************************
 ** Function: handleInputMaze
 ** Description: Handles user input when they're focused on the maze (no alert showing).
 ** Parameters: chr (int) - the character the user just inputted
 **				nextPanel (Panel*&) - the next panel to show, if necessary 
 ** Pre-Conditions: none.
 ** Post-Conditions: Returns true if it's time to move on to another panel. Redraws 
 **					 everything that needs to be redrawn otherwise. 
 ****************************************************************************************/
bool Game::handleInputMaze(int chr, Panel*& nextPanel) {
	switch(chr) {
		case 'q':
			// quit
			alertType = ALERT_QUIT;
			initAlertWin();
			drawAlert();
			return false;
		case 'i':
			moveCameraUpDown(true);
			break;
		case 'j':
			moveCameraLeftRight(true);
			break;
		case 'k':
			moveCameraUpDown(false);
			break;
		case 'l':
			moveCameraLeftRight(false);
			break;
		case 'p':
		case 'w': case KEY_UP:
		case 'a': case KEY_LEFT:
		case 's': case KEY_DOWN:
		case 'd': case KEY_RIGHT:
			if (maze.getUsingAI()) return false;
		case ' ':
		case '\n': case '\r': case KEY_ENTER:
			// counts as a turn, need to pass to the maze
			if (takeTurn(chr)) return false;
			break;
	}

	// HUD is already refreshed in the redraw method
	drawMaze();
	redrawHudLeft();
	return false;
}

/*****************************************************************************************
 ** Function: handleInputAlert
 ** Description: Handles user input when they're focused on an alert.
 ** Parameters: chr (int) - the character the user just inputted
 **				nextPanel (Panel*&) - the next panel to show, if necessary 
 ** Pre-Conditions: none.
 ** Post-Conditions: Returns true if it's time to move on to another panel. Redraws 
 **					 everything that needs to be redrawn otherwise. 
 ****************************************************************************************/
bool Game::handleInputAlert(int chr, Panel*& nextPanel) {
	switch(chr) {
		case 'q':
			// quit
			nextPanel = nullptr;
			return true;
		case 'a': case KEY_LEFT:
			redrawAlert((alertOption + alertNumOptions - 1) % alertNumOptions);
			break;
		case 'd': case KEY_RIGHT:
			redrawAlert((alertOption + 1) % alertNumOptions);
			break;
		case ' ':
		case '\n': case '\r': case KEY_ENTER:
			// handle based on type of alert
			return handleEnterAlert(nextPanel);
	}
	return false;
}

/*****************************************************************************************
 ** Function: handleEnterAlert
 ** Description: Handles an enter keystroke when the user is focused on an alert.
 ** Parameters: nextPanel (Panel*&) - the next panel to show, if necessary 
 ** Pre-Conditions: none.
 ** Post-Conditions: Returns true if it's time to move on to another panel. Redraws 
 **					 everything that needs to be redrawn otherwise. 
 ****************************************************************************************/
bool Game::handleEnterAlert(Panel*& nextPanel) {
	switch(alertType) {
		case ALERT_QUIT:
			// left is yes, right is no
			if (alertOption == 0) {
				nextPanel = new StartPanel(maze.getMazeHeight());
				return true;
			}
			closeAlert();
			return false;
		case ALERT_WON:
		case ALERT_LOST:
			// left is again, right is quit 
			if (alertOption == 0) {
				nextPanel = new Game(maze.getMazeWidth(), maze.getMazeHeight(), false);
				return true;
			}
			nextPanel = new StartPanel(maze.getMazeWidth(), maze.getMazeHeight());
			return true;
		case ALERT_TUTORIAL_ASK:
			// depends on enabled settings (2 = fog if enabled, 2 = ai if enabled and fog 
			// isn't, 3 = ai if both fog and ai enabled)
			switch(alertOption) {
				case 0:
					closeAlert();
					return false;		// no new win
				case 1:
					alertType = ALERT_TUTORIAL_1;
					break;
				case 2:
					if (maze.getUsingFog()) {
						alertType = ALERT_TUTORIAL_FOG;
						break;
					}
					// continue down and init AI tutorial
				case 3:
					alertType = ALERT_TUTORIAL_AI;
					break;
			}
			break;
		case ALERT_TUTORIAL_1:
			alertType = ALERT_TUTORIAL_2;
			break;
		case ALERT_TUTORIAL_2:
			alertType = ALERT_TUTORIAL_3;
			break;
		case ALERT_TUTORIAL_3:
			alertType = ALERT_TUTORIAL_4;
			break;
		case ALERT_TUTORIAL_4:
			alertType = ALERT_TUTORIAL_5;
			break;
		case ALERT_TUTORIAL_5:
			alertType = ALERT_TUTORIAL_6;
			break;
		case ALERT_TUTORIAL_6:
		case ALERT_TUTORIAL_FOG:
		case ALERT_TUTORIAL_AI:
			if (alertOption == 0) {
				// no more tutorials
				closeAlert();
				return false;
			}
			alertType = ALERT_TUTORIAL_ASK;		// going back
			break;
	}
	// new alert (init deletes the previous, so don't need to)
	initAlertWin();
	drawAlert();
	return false;
}

void Game::draw() {
	initHudWin();
	drawHud();
	wrefresh(hudWin);

	initCamera();
	
	initMazeWin();
	drawMaze();
	wrefresh(mazeWin);

	if (alertWin != nullptr) {
		initAlertWin();
		drawAlert();
	}
}

/*****************************************************************************************
 ** Function: initHudWin
 ** Description: Initializes the curses window for the HUD. 
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: hudWin is not null.
 ****************************************************************************************/
void Game::initHudWin() {
	if (hudWin != nullptr) {
		wclear(hudWin);
		delwin(hudWin);
	}
	hudWin = newwin(GAME_HUD_HEIGHT, 0, 0, 0);
	intrflush(hudWin, true);
	keypad(hudWin, true);
}

/*****************************************************************************************
 ** Function: initMazeWin
 ** Description: Initializes the curses window for the maze. 
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: mazeWin is not null.
 ****************************************************************************************/
void Game::initMazeWin() {
	if (mazeWin != nullptr) {
		wclear(mazeWin);
		delwin(mazeWin);
	}
	mazeWin = newwin(0, 0, GAME_HUD_HEIGHT, 0);
	intrflush(mazeWin, true);
	keypad(mazeWin, true);
}

/*****************************************************************************************
 ** Function: initAlertWin
 ** Description: Initializes the curses window for the alert. 
 ** Parameters: none.
 ** Pre-Conditions: The correct alert type should be stored in alertType.
 ** Post-Conditions: alertWin is not null. alertNumOptions is correct and alertOption is 
 **					 set to a default option.
 ****************************************************************************************/
void Game::initAlertWin() {
	if (alertWin != nullptr) closeAlert();

	int rows = getAlertRows(), cols = getAlertCols();
	alertWin = newwin(rows, cols, (LINES - rows)/2, (COLS - cols)/2);
	intrflush(alertWin, true);
	keypad(alertWin, true);

	alertNumOptions = getAlertButtons().size();
	setDefaultAlertAction();
}

/*****************************************************************************************
 ** Function: drawHud
 ** Description: Draws the entire HUD.
 ** Parameters: none.
 ** Pre-Conditions: hudWin has already been initialized.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Game::drawHud() {
	werase(hudWin);

	if (COLS < 2*GAME_HUD_WIDTH_1 || COLS < 2*GAME_HUD_WIDTH_2) {
		// 1:1 aspect ratio not possible, so give all slack to the smaller side
		if (GAME_HUD_WIDTH_1 < GAME_HUD_WIDTH_2) hudRightCol = COLS - GAME_HUD_WIDTH_2; 
		else hudRightCol = GAME_HUD_WIDTH_1;
	}
	else {
		// aspect ratio should be 1:1 (and is possible)
		hudRightCol = COLS / 2;
	}
	drawHudLeft();
	drawHudRight();

	mvwhline(hudWin, 4, 0, 0, COLS);
}

/*****************************************************************************************
 ** Function: drawHudLeft
 ** Description: Draws the left half of the HUD.
 ** Parameters: none.
 ** Pre-Conditions: hudWin has already been initialized.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Game::drawHudLeft() {
	mvwaddstr(hudWin, 0, 0, GAME_HUD_TEXT_QUIT);
	mvwaddstr(hudWin, 2, 0, (GAME_HUD_TEXT_SKILLS + std::to_string(maze.getNumSkills())).c_str());
	mvwaddstr(hudWin, 3, 0, (GAME_HUD_TEXT_TA + TA::getStatus()).c_str());
}

/*****************************************************************************************
 ** Function: redrawHudLeft
 ** Description: Redraws the left half of the HUD.
 ** Parameters: none.
 ** Pre-Conditions: hudWin has already been initialized.
 ** Post-Conditions: hudWin has been refreshed.
 ****************************************************************************************/
void Game::redrawHudLeft() {
	// write and delete until the right col 
	std::string str = std::to_string(maze.getNumSkills());
	mvwaddstr(hudWin, 2, sizeof(GAME_HUD_TEXT_SKILLS) - 1, str.c_str());
	for (int i = sizeof(GAME_HUD_TEXT_SKILLS)  - 1 + str.length(); i < hudRightCol; i++) 
		waddch(hudWin, ' ');

	str = TA::getStatus();
	mvwaddstr(hudWin, 3, sizeof(GAME_HUD_TEXT_TA) - 1, str.c_str());
	for (int i = sizeof(GAME_HUD_TEXT_TA) - 1 + str.length(); i < hudRightCol; i++) 
		waddch(hudWin, ' ');

	wrefresh(hudWin);
}

/*****************************************************************************************
 ** Function: drawHudRight
 ** Description: Draws the right half of the HUD.
 ** Parameters: none.
 ** Pre-Conditions: hudWin has already been initialized.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Game::drawHudRight() {
	if (maze.getUsingAI()) {
		mvwaddstr(hudWin, 1, hudRightCol, GAME_HUD_TEXT_AI);
		mvwaddstr(hudWin, 3, hudRightCol, GAME_HUD_TEXT_CAMERA);
	}
	else {
		mvwaddstr(hudWin, 1, hudRightCol, GAME_HUD_TEXT_MOVE);
		mvwaddstr(hudWin, 2, hudRightCol, GAME_HUD_TEXT_CAMERA);
		mvwaddstr(hudWin, 3, hudRightCol, GAME_HUD_TEXT_DEMO);
	}
}

/*****************************************************************************************
 ** Function: drawMaze
 ** Description: Draws the entire maze.
 ** Parameters: none.
 ** Pre-Conditions: mazeWin has already been initialized.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Game::drawMaze() {
	werase(mazeWin);
	
	// loop through the relevant squares 
	int minRow = std::max(cameraRow/GAME_SQUARE_SIZE, 0);
	int maxRow = std::min((cameraRow+mazeWinRows)/GAME_SQUARE_SIZE, maze.getMazeHeight() - 1);
	int minCol = std::max(cameraCol/GAME_SQUARE_SIZE, 0); 
	int maxCol = std::min((cameraCol + COLS)/GAME_SQUARE_SIZE, maze.getMazeWidth() - 1);
	for (int r = minRow; r <= maxRow; r++) {
		for (int c = minCol; c <= maxCol; c++) {
			drawSquare(r, c);
		}
	}

	drawMazeBorder();

	// draw all people and check for relevancy
	for (int i = maze.getNumPeople() - 1; i >= 0; i--) {
		if (!maze.getInFog(i)) drawPerson(maze.getPerson(i));
	}
}

/*****************************************************************************************
 ** Function: drawSquare
 ** Description: Draws a single square in the maze.
 ** Parameters: mazeRow (int) - the row of the square to draw
 **				mazeCol (int) - the column of the square to draw
 ** Pre-Conditions: mazeWin has already been initialized.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Game::drawSquare(int mazeRow, int mazeCol) {
	int currCol = mazeCol*GAME_SQUARE_SIZE - cameraCol, currRow = mazeRow*GAME_SQUARE_SIZE - cameraRow;
	
	const MazeLocation* loc = maze.getLoc(mazeRow, mazeCol);
	if (loc == nullptr) return;
	int ch = loc->getDisplayChar(maze.getInFog(mazeRow, mazeCol));

	for (int i = 0; i < GAME_SQUARE_SIZE; i++) {
		for (int j = 0; j < GAME_SQUARE_SIZE; j++) {
			if (currRow + i >= 0 && currCol + j >= 0 
				&& currRow + i < mazeWinRows && currCol + j < COLS) {
				mvwaddch(mazeWin, currRow + i, currCol + j, ch);
			}
		}
	}
}

/*****************************************************************************************
 ** Function: drawPerson
 ** Description: Draws a single person in the maze.
 ** Parameters: person (const MazePerson*) - the person to draw
 ** Pre-Conditions: mazeWin has already been initialized.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Game::drawPerson(const MazePerson* person) {
	// draws a plus pattern GAME_SQUARE_SIZE/3 wide centered on the square 
	// width of the plus pattern 
	int width = GAME_SQUARE_SIZE / 3;

	// top left display row/col 
	int row = person->getRow()*GAME_SQUARE_SIZE - cameraRow;
	int col = person->getCol()*GAME_SQUARE_SIZE - cameraCol;
	int ch = person->getDisplayChar();

	for (int i = 0; i < width; i++) {
		int currRow, currCol;
		// horizontal part (row is const)
		currRow = row + width + i;
		for (int j = 0; j < GAME_SQUARE_SIZE; j++) {
			currCol = col + j;
			if (currRow >= 0 && currCol >= 0 && currRow < mazeWinRows && currCol < COLS) {
				mvwaddch(mazeWin, currRow, currCol, ch);
			}
		}

		// vertical part (col is const)
		currCol = col + width + i;
		for (int j = 0; j < GAME_SQUARE_SIZE; j++) {
			currRow = row + j;
			if (currRow >= 0 && currCol >= 0 && currRow < mazeWinRows && currCol < COLS) {
				mvwaddch(mazeWin, currRow, currCol, ch);
			}
		}
	}
}

/*****************************************************************************************
 ** Function: drawMazeBorder
 ** Description: Draws the border of the maze, if necessary
 ** Parameters: none.
 ** Pre-Conditions: mazeWin has already been initialized.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Game::drawMazeBorder() {
	// display rows for top/bottom borders
	int top = -1 - cameraRow, bottom = GAME_SQUARE_SIZE*maze.getMazeHeight() + 1 - cameraRow;
	// display cols for left/right borders
	int left = -1 - cameraCol, right = GAME_SQUARE_SIZE*maze.getMazeWidth() + 1 - cameraCol;

	// draw checks
	if (top >= 0) {
		// top border
		for (int i = std::max(left+1, 0); i < std::min(right, COLS); i++) 
			mvwaddch(mazeWin, top, i, ACS_HLINE);
	}
	if (left >= 0) {
		// left border
		for (int i = std::max(top+1, 0); i < std::min(bottom, mazeWinRows); i++) 
			mvwaddch(mazeWin, i, left, ACS_VLINE);
		
		// left corner checks
		if (top >= 0) mvwaddch(mazeWin, top, left, ACS_ULCORNER);
		if (bottom < mazeWinRows) mvwaddch(mazeWin, bottom, left, ACS_LLCORNER);
	}
	if (bottom < mazeWinRows) {
		// bottom border
		for (int i = std::max(left+1, 0); i < std::min(right, COLS); i++) 
			mvwaddch(mazeWin, bottom, i, ACS_HLINE);
	}
	if (right < COLS) {
		// right border
		for (int i = std::max(top+1, 0); i < std::min(bottom, mazeWinRows); i++) 
			mvwaddch(mazeWin, i, right, ACS_VLINE);
		
		// right corner checks
		if (top >= 0) mvwaddch(mazeWin, top, right, ACS_URCORNER);
		if (bottom < mazeWinRows) mvwaddch(mazeWin, bottom, right, ACS_LRCORNER);
	}
}

/*****************************************************************************************
 ** Function: takeTurn
 ** Description: Takes a single turn in the maze. Moves everybody and refreshes everything
 **				 that needs refreshing on the screen.
 ** Parameters: input (int) - the input char given by the user
 ** Pre-Conditions: mazeWin has already been initialized.
 ** Post-Conditions: none.
 ****************************************************************************************/
bool Game::takeTurn(int input) {
	// move everybody and redraw stuff 
	maze.takeTurn(input);

	// win/lose condition
	bool won;
	if (maze.checkGameEnd(won)) {
		// show ending screen
		redrawHudLeft();
		drawMaze();
		wrefresh(hudWin);
		wrefresh(mazeWin);
		
		// make alert
		alertType = won ? ALERT_WON : ALERT_LOST;
		initAlertWin();
		drawAlert();
		return true;
	}
	return false;
}

/*****************************************************************************************
 ** Function: getAlertCols
 ** Description: Gets the number of columns wide the alert is or needs to be.
 ** Parameters: none.
 ** Pre-Conditions: If the alert hasn't been made yet, requires alertType to be filled in 
 **					correctly. 
 ** Post-Conditions: none.
 ****************************************************************************************/
int Game::getAlertCols() {
	if (alertWin == nullptr) {
		const std::vector<std::string> text = getAlertText(), buttons = getAlertButtons();
		if (text.size() == 0 || buttons.size() == 0) return 0;

		// max width of text (min size to fit the text)
		int max = 0;
		for (std::string t : text) 
			if (t.length() > max) max = t.length();
		max += 2*ALERT_MARGIN;

		// max width of a given button
		int buttonMax = 0;
		for (std::string t : buttons) 
			if (t.length() > buttonMax) buttonMax = t.length();
		// total minimum size of buttons line (they're equally spaced)
		// border doesn't count in min button space, so added separately
		buttonMax = (buttonMax + ALERT_MIN_BUTTON_SPACE)*buttons.size() 
					+ ALERT_MIN_BUTTON_SPACE + 2;
		
		if (buttonMax > max) max = buttonMax;

		return max;
	}

	// otherwise, already did alert setup, so just poll the alert window
	int rows, cols;
	getmaxyx(alertWin, rows, cols);
	return cols;
}

/*****************************************************************************************
 ** Function: getAlertRows
 ** Description: Gets the number of rows tall the alert is or needs to be.
 ** Parameters: none.
 ** Pre-Conditions: If the alert hasn't been made yet, requires alertType to be filled in
 **					correctly. 
 ** Post-Conditions: none.
 ****************************************************************************************/
int Game::getAlertRows() {
	// 3 margins: above text, between text and buttons, and below buttons
	if (alertWin == nullptr) return 3*ALERT_ELEMENT_MARGIN + getAlertText().size() + 1;

	// otherwise, already did alert setup, so just poll the alert window
	int rows, cols;
	getmaxyx(alertWin, rows, cols);
	return rows;
}

/*****************************************************************************************
 ** Function: getAlertText
 ** Description: Gets all of the main alert text for the current alert.
 ** Parameters: none.
 ** Pre-Conditions: alertType should be filled in correctly.
 ** Post-Conditions: none.
 ****************************************************************************************/
const std::vector<std::string> Game::getAlertText() {
	std::vector<std::string> text;
	switch(alertType) {
		case ALERT_QUIT:
			text.push_back(ALERT_QUIT_TEXT);
			break;
		case ALERT_WON:
			text.push_back(ALERT_WON_TEXT);
			break;
		case ALERT_LOST:
			text.push_back(ALERT_LOST_TEXT);
			break;
		case ALERT_TUTORIAL_ASK:
			text.push_back(ALERT_TUTORIAL_ASK_TEXT);
			break;
		case ALERT_TUTORIAL_1:
			text.push_back(ALERT_TUTORIAL_1_TEXT_1);
			text.push_back(ALERT_TUTORIAL_1_TEXT_2);
			text.push_back(ALERT_TUTORIAL_1_TEXT_3);
			text.push_back(ALERT_TUTORIAL_1_TEXT_4);
			break;
		case ALERT_TUTORIAL_2:
			text.push_back(ALERT_TUTORIAL_2_TEXT_1);
			text.push_back(ALERT_TUTORIAL_2_TEXT_2);
			text.push_back(ALERT_TUTORIAL_2_TEXT_3);
			text.push_back(ALERT_TUTORIAL_2_TEXT_4);
			text.push_back(ALERT_TUTORIAL_2_TEXT_5);
			break;
		case ALERT_TUTORIAL_3:
			text.push_back(ALERT_TUTORIAL_3_TEXT_1);
			text.push_back(ALERT_TUTORIAL_3_TEXT_2);
			text.push_back(ALERT_TUTORIAL_3_TEXT_3);
			text.push_back(ALERT_TUTORIAL_3_TEXT_4);
			text.push_back(ALERT_TUTORIAL_3_TEXT_5);
			text.push_back(ALERT_TUTORIAL_3_TEXT_6);
			text.push_back(ALERT_TUTORIAL_3_TEXT_7);
			break;
		case ALERT_TUTORIAL_4:
			text.push_back(ALERT_TUTORIAL_4_TEXT_1);
			text.push_back(ALERT_TUTORIAL_4_TEXT_2);
			text.push_back(ALERT_TUTORIAL_4_TEXT_3);
			text.push_back(ALERT_TUTORIAL_4_TEXT_4);
			text.push_back(ALERT_TUTORIAL_4_TEXT_5);
			text.push_back(ALERT_TUTORIAL_4_TEXT_6);
			text.push_back(ALERT_TUTORIAL_4_TEXT_7);
			break;
		case ALERT_TUTORIAL_5:
			text.push_back(ALERT_TUTORIAL_5_TEXT_1);
			text.push_back(ALERT_TUTORIAL_5_TEXT_2);
			text.push_back(ALERT_TUTORIAL_5_TEXT_3);
			text.push_back(ALERT_TUTORIAL_5_TEXT_4);
			text.push_back(ALERT_TUTORIAL_5_TEXT_5);
			break;
		case ALERT_TUTORIAL_6:
			text.push_back(ALERT_TUTORIAL_6_TEXT_1);
			text.push_back(ALERT_TUTORIAL_6_TEXT_2);
			text.push_back(ALERT_TUTORIAL_6_TEXT_3);
			text.push_back(ALERT_TUTORIAL_6_TEXT_4);
			break;
		case ALERT_TUTORIAL_FOG:
			text.push_back(ALERT_TUTORIAL_FOG_TEXT_1);
			text.push_back(ALERT_TUTORIAL_FOG_TEXT_2);
			text.push_back(ALERT_TUTORIAL_FOG_TEXT_3);
			text.push_back(ALERT_TUTORIAL_FOG_TEXT_4);
			text.push_back(ALERT_TUTORIAL_FOG_TEXT_5);
			text.push_back(ALERT_TUTORIAL_FOG_TEXT_6);
			break;
		case ALERT_TUTORIAL_AI:
			text.push_back(ALERT_TUTORIAL_AI_TEXT_1);
			text.push_back(ALERT_TUTORIAL_AI_TEXT_2);
			text.push_back(ALERT_TUTORIAL_AI_TEXT_3);
			text.push_back(ALERT_TUTORIAL_AI_TEXT_4);
			text.push_back(ALERT_TUTORIAL_AI_TEXT_5);
			text.push_back(ALERT_TUTORIAL_AI_TEXT_6);
			break;
	}
	return text;
}

/*****************************************************************************************
 ** Function: getAlertButtons
 ** Description: Gets all of the buttons required for the current alert.
 ** Parameters: none.
 ** Pre-Conditions: alertType should be filled in correctly.
 ** Post-Conditions: none.
 ****************************************************************************************/
const std::vector<std::string> Game::getAlertButtons() {
	std::vector<std::string> buttons;
	switch(alertType) {
		case ALERT_QUIT:
			buttons.push_back(ALERT_BUTTON_YES);
			buttons.push_back(ALERT_BUTTON_NO);
			break;
		case ALERT_WON:
		case ALERT_LOST:
			buttons.push_back(ALERT_BUTTON_AGAIN);
			buttons.push_back(ALERT_BUTTON_QUIT);
			break;
		case ALERT_TUTORIAL_ASK:
			buttons.push_back(ALERT_BUTTON_NONE);
			buttons.push_back(ALERT_BUTTON_NORMAL_TUT);
			if (maze.getUsingFog()) buttons.push_back(ALERT_BUTTON_FOG_TUT);
			if (maze.getUsingAI()) buttons.push_back(ALERT_BUTTON_AI_TUT);
			break;
		case ALERT_TUTORIAL_1:
		case ALERT_TUTORIAL_2:
		case ALERT_TUTORIAL_3:
		case ALERT_TUTORIAL_4:
		case ALERT_TUTORIAL_5:
			buttons.push_back(ALERT_BUTTON_OK);
			break;
		case ALERT_TUTORIAL_6:
		case ALERT_TUTORIAL_FOG:
		case ALERT_TUTORIAL_AI:
			buttons.push_back(ALERT_BUTTON_PLAY);
			buttons.push_back(ALERT_BUTTON_ANOTHER);
			break;
	}
	return buttons;
}

/*****************************************************************************************
 ** Function: setDefaultAlertAction
 ** Description: Sets the default action for the current alert.
 ** Parameters: none.
 ** Pre-Conditions: alertType should be filled in correctly.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Game::setDefaultAlertAction() {
	switch(alertType) {
		case ALERT_QUIT:
			alertOption = 1;	// the "no" button (stay)
			break;
		// play again
		case ALERT_WON:
		case ALERT_LOST:
		// no tutorial button
		case ALERT_TUTORIAL_ASK:
		// the only button
		case ALERT_TUTORIAL_1:
		case ALERT_TUTORIAL_2:
		case ALERT_TUTORIAL_3:
		case ALERT_TUTORIAL_4:
		case ALERT_TUTORIAL_5:
		// button to play
		case ALERT_TUTORIAL_6:
		case ALERT_TUTORIAL_FOG:
		case ALERT_TUTORIAL_AI:
			alertOption = 0;
			break;
	}
}

/*****************************************************************************************
 ** Function: drawAlert
 ** Description: Draws the current alert.
 ** Parameters: none.
 ** Pre-Conditions: alertWin should be initialized and alertType should be filled in 
 **					correctly.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Game::drawAlert() {
	const std::vector<std::string> text = getAlertText(), buttons = getAlertButtons();
	
	wclear(alertWin);
	box(alertWin, 0, 0);

	for (int i = 0; i < text.size(); i++)
		mvwaddstr(alertWin, ALERT_ELEMENT_MARGIN + i, ALERT_MARGIN, text[i].c_str());

	int row = 2*ALERT_ELEMENT_MARGIN + text.size();
	for (int i = 0; i < buttons.size(); i++) drawAlertButton(buttons, i, row);

	wrefresh(alertWin);
}

/*****************************************************************************************
 ** Function: redrawAlert
 ** Description: Redraws the current alert with the new option.
 ** Parameters: newOption (int) - the new option to set alertOption to 
 ** Pre-Conditions: alertWin should be initialized.
 ** Post-Conditions: alertOption is now equal to newOption.
 ****************************************************************************************/
void Game::redrawAlert(int newOption) {
	const std::vector<std::string> buttons = getAlertButtons();
	int row = 2*ALERT_ELEMENT_MARGIN + getAlertText().size();

	int temp = alertOption;
	alertOption = newOption;
	drawAlertButton(buttons, temp, row);
	if (temp != alertOption) drawAlertButton(buttons, alertOption, row);
}

/*****************************************************************************************
 ** Function: drawAlertButton
 ** Description: Draws a single button for the alert. 
 ** Parameters: buttons (const vector<string>&) - the buttons to draw
 **				optionPos (int) - the position of the option to draw 
 **				row (int) - the row to draw it on 
 ** Pre-Conditions: alertWin should be initialized.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Game::drawAlertButton(const std::vector<std::string>& buttons, int optionPos, int row) {
	if (optionPos < 0 || optionPos >= buttons.size()) return;
	int attr = alertOption == optionPos ? A_STANDOUT : 0;
	
	// how wide each button is 
	int buttonCols = (getAlertCols() - 2)/buttons.size();
	// get button offset, center within button, then add the border back
	int col = buttonCols*optionPos + (buttonCols - buttons[optionPos].length() + 1)/2 + 1;

	wmove(alertWin, row, col);
	Util::drawString(alertWin, buttons[optionPos], attr);
}

/*****************************************************************************************
 ** Function: closeAlert
 ** Description: Closes the alert and frees all of its related memory.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Game::closeAlert() {
	if (alertWin == nullptr) return;

	delwin(alertWin);
	alertWin = nullptr;

	drawHud();
	wrefresh(hudWin);
	drawMaze();
	wrefresh(mazeWin);
}

/*****************************************************************************************
 ** Function: minCameraRow
 ** Description: Gets the lowest row the camera display at.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int Game::minCameraRow() const { return -mazeWinRows/2; }

/*****************************************************************************************
 ** Function: maxCameraRow
 ** Description: Gets the highest row the camera can display at.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int Game::maxCameraRow() const {
	return GAME_SQUARE_SIZE*maze.getMazeHeight() - mazeWinRows/2;
}

/*****************************************************************************************
 ** Function: minCameraCol
 ** Description: Gets the lowest column the camera can display at.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int Game::minCameraCol() const { return -COLS/2; }

/*****************************************************************************************
 ** Function: maxCameraCol
 ** Description: Gets the highest column the camera can display at.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int Game::maxCameraCol() const {
	return GAME_SQUARE_SIZE*maze.getMazeWidth() - COLS/2;
}

/*****************************************************************************************
 ** Function: initCamera
 ** Description: Initializes the row and column of the camera. If the maze is too small in 
 **				 a dimension (all of it can fit), this will center the maze along that 
 **				 direction. Otherwise, this centers it on the player as much as possible.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: cameraRow and cameraCol are initialized.
 ****************************************************************************************/
void Game::initCamera() {
	// physical row/col of the student (center)
	int stuRow = GAME_SQUARE_SIZE*maze.getStudent()->getRow() + GAME_SQUARE_SIZE/2;
	int stuCol = GAME_SQUARE_SIZE*maze.getStudent()->getCol() + GAME_SQUARE_SIZE/2;

	// center on the maze if the entire maze fits on the screen 
	// otherwise, center on the player (at least as much as possible)
	if (mazeWinRows > GAME_SQUARE_SIZE*maze.getMazeHeight()) 
		cameraRow = (GAME_SQUARE_SIZE*maze.getMazeHeight() - mazeWinRows)/2;
	else {
		cameraRow = std::min(stuRow - mazeWinRows/2, maxCameraRow());
		cameraRow = std::max(minCameraRow(), cameraRow);
	}

	if (COLS > GAME_SQUARE_SIZE*maze.getMazeWidth()) 
		cameraCol = (GAME_SQUARE_SIZE*maze.getMazeWidth() - COLS)/2;
	else {
		cameraCol = std::min(stuCol - COLS/2, maxCameraCol());
		cameraCol = std::max(minCameraCol(), cameraCol);
	}
}

/*****************************************************************************************
 ** Function: moveCameraLeftRight
 ** Description: Moves the camera left or right, depending on what the caller wants.
 ** Parameters: left (bool) - true means to move the camera left, false means right
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Game::moveCameraLeftRight(bool left) {
	// the maze is too small to care (should already be centered on maze)
	if (COLS > GAME_SQUARE_SIZE*maze.getMazeWidth()) return;
	
	
	if (left) cameraCol = std::max(cameraCol - 1, minCameraCol());
	else cameraCol = std::min(cameraCol + 1, maxCameraCol());
}

/*****************************************************************************************
 ** Function: moveCameraUpDown
 ** Description: Moves the camera up or down, depending on what the caller wants.
 ** Parameters: up (bool) - true means to move the camera up, false means down
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Game::moveCameraUpDown(bool up) {
	// the maze is too small to care (should already be centered on maze)
	if (mazeWinRows > GAME_SQUARE_SIZE*maze.getMazeHeight()) return;
	
	if (up) cameraRow = std::max(cameraRow - 1, minCameraRow());
	else cameraRow = std::min(cameraRow + 1, maxCameraRow());
}
