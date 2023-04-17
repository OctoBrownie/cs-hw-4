/*****************************************************************************************
 ** Program Filename: game.h
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Header file for the Game class, which manages the panel for the game,
 **				 meaning it mostly handles input and drawing to the screen.  
 ** Input: Gets the maze sizes from the start panel.
 ** Output: Doesn't really output anything to any other panels, just draws everything for 
 **			the maze and HUD and everything to screen.
 ****************************************************************************************/
#ifndef __game_h__
#define __game_h__

#include<string>
#include<ncurses.h>

#include "panel.h"
#include "maze.h"
#include "mazeperson.h"

class Game : public Panel {
	protected: 
		WINDOW *hudWin, *mazeWin, *alertWin;
		Maze maze; 
		int mazeWinRows;
		// num cols are always the cols of the window (just use COLS) 

		// top left corner relative to the top left corner of the maze (display coords)
		// coords of something relative to the camera: actual row/col - camera row/col
		int cameraRow, cameraCol;

		// col num where the right column starts for the HUD  
		int hudRightCol;

		int alertType, alertOption, alertNumOptions;

		bool askTutorial;

		// return true if we're going to another panel 
		bool handleInputMaze(int chr, Panel*& nextPanel);
		bool handleInputAlert(int chr, Panel*& nextPanel);
		bool handleEnterAlert(Panel*& nextPanel);

		void initHudWin();
		void initMazeWin();
		void initAlertWin();

		virtual void draw() override;

		void drawHud();
		void drawHudLeft();
		void redrawHudLeft();	// only need to redraw the left HUD since the right is completely static
		void drawHudRight();
		
		void drawMaze();
		void drawSquare(int mazeRow, int mazeCol);
		void drawPerson(const MazePerson* person);
		void drawMazeBorder();

		bool takeTurn(int input);

		void initCamera();
		void moveCameraLeftRight(bool left);
		void moveCameraUpDown(bool up);
		
		int minCameraRow() const;
		int maxCameraRow() const;
		int minCameraCol() const;
		int maxCameraCol() const;

		const std::vector<std::string> getAlertText();
		const std::vector<std::string> getAlertButtons();
		int getAlertRows();
		int getAlertCols();
		
		void setDefaultAlertAction();
		void drawAlert();
		void drawAlertButton(const std::vector<std::string>& buttons, int optionPos, 
							 int row);
		
		void redrawAlert(int newOption);
		void closeAlert();
	public:
		Game(int mazeSize);
		Game(int mazeWidth, int mazeHeight);
		Game(int mazeWidth, int mazeHeight, bool askTutorial);
		virtual ~Game();

		virtual void init() override;
		virtual Panel* loop() override;
		virtual void resizeHandler() override;
};

#endif 