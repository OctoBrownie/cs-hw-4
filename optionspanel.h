/*****************************************************************************************
 ** Program Filename: optionspanel.cpp
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Header file for the OptionsPanel class, which is the panel showing the 
 **				 settings the user can change.
 ** Input: Depends on the function, but usually nothing.
 ** Output: Depends on the function, but it draws things to screen or changes things about
 **			the panel.
 ****************************************************************************************/
#ifndef __optionspanel_h__
#define __optionspanel_h__

#include "panel.h"

class OptionsPanel : public Panel {
	private: 
		int currOption;
		int difficulty;
		bool fogOfWar, useAI;

		virtual void draw() override;

		void drawDescription(int button);
		void drawButton(int button);
		void redrawButton(int newOption);

		int getSpace();

		void handleLeftRightInput(bool left);
		bool handleEnterInput(Panel*& nextPanel);

		void saveToFile();
	public:
		static void loadFromFile(int& diff, bool& fog, bool& ai);

		OptionsPanel();
		virtual ~OptionsPanel();

		// virtual void resizeHandler() override;
		virtual void init() override;
		virtual Panel* loop() override;
};

#endif 