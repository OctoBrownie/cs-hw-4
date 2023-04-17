/*****************************************************************************************
 ** Program Filename: main.cpp
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: The implementation of the main function for the entire program.
 ** Input: Takes user inputs from the terminal. 
 ** Output: Outputs all of the game things to terminal.
 ****************************************************************************************/
#include<random>
#include<time.h>
#include<ncurses.h>
#include<string>

#include "panel.h"
#include "startpanel.h"

int main() {
	srand(time(nullptr));

	// curses init 
	initscr();
	start_color();

	// curses customization
	noecho();
	intrflush(stdscr, true);
	keypad(stdscr, true);
	curs_set(0);

	Panel* p = new StartPanel();

	while (p != nullptr) {
		p->init();
		Panel* temp = p->loop();
		delete p;
		p = temp;
	}
	endwin();
	return 0;
}