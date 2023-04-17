/*****************************************************************************************
 ** Program Filename: panel.h
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Header file for the Panel class, an abstract class that represents a
 **				 single screen.
 ** Input: Depends on the function, but usually nothing.
 ** Output: Depends on the function, but generally changes some things about the screen.
 ****************************************************************************************/
#ifndef __panel_h__
#define __panel_h__

class Panel {
	protected: 
		// width is handled by COLS and height by LINES (both defined in ncurses)
		bool windowDisabled;
		
		virtual void draw() = 0;
	public: 
		virtual ~Panel();
		// no copy constructor or AOO needed (only one used at a time, always pass by ref)

		virtual void resizeHandler();
		virtual void init();
		virtual Panel* loop() = 0;
};

#endif