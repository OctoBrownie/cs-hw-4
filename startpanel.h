/*****************************************************************************************
 ** Program Filename: startpanel.h
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Header file for the StartPanel class, which represents the home screen
 **				 for the user, where they can start a new game or change settings. 
 ** Input: Depends on the function, but usually nothing.
 ** Output: Depends on the function, but generally changes some things about the screen or
 **			updates internal variables.
 ****************************************************************************************/
#ifndef __startpanel_h__
#define __startpanel_h__

#include<string>

#include "panel.h"

class StartPanel : public Panel {
	protected:
		int mazeWidth, mazeHeight;
		int currButton;
		bool isSquare;

		virtual void draw() override;

		void drawButton(int button);
		void drawField(int row, bool isWidth, int attr);
		void drawIsSquare(int row, int attr);

		void changeMazeSize(bool increment);
		void selectNextButton(bool goUp);
		bool handleEnter(Panel*& nextPanel);

		int calculateSpace();
	public:
		StartPanel();
		StartPanel(int mazeSize);
		StartPanel(int mazeWidth, int mazeHeight);

		virtual void init() override;
		virtual Panel* loop() override;
};

#endif 