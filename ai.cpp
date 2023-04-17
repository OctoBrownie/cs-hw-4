/*****************************************************************************************
 ** Program Filename: ai.cpp
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Implementation file for the AI class. 
 ** Input: For the only function defined here, it's the map of the entire maze plus the 
 **		   people in it.
 ** Output: For the only function defined here, it's the move the AI wants to take. 
 ****************************************************************************************/
#include<random>

#include "consts.h"
#include "ai.h"

/*****************************************************************************************
 ** Function: getChar
 ** Description: Gets the move the AI wants to take, in the form of a normal character 
 **				 that the user could've typed to do the same action (ex: outputs 'w' if 
 **				 the AI wants to go up). 
 ** Parameters: map (vector<vector<MazeLocation*>>&) is the map of the maze right now
 **				people (vector<MazePerson*>&) is the list of people in the maze right now
 ** Pre-Conditions: none.
 ** Post-Conditions: The return value is a character that corresponds to a normal user 
 **					 input for moving the character (WASD or space) or using a skill (P).
 ****************************************************************************************/
int AI::getChar(const std::vector<std::vector<MazeLocation*>>& map, 
				const std::vector<MazePerson*>& people) const {
	int choice;
	do {
		choice = rand() % 5;
		switch(choice) {
			// go up
			case 0:
				if (row != 0 && map[row-1][col]->isOccupiable()) return 'w';
				break;
			// go left
			case 1:
				if (col != 0 && map[row][col-1]->isOccupiable()) return 'a';
				break;
			// go down 
			case 2:
				if (row != map.size()-1 && map[row+1][col]->isOccupiable()) return 's';
				break;
			// go right
			case 3:
				if (col != map[0].size()-1 && map[row][col+1]->isOccupiable()) return 'd';
				break;
			// demo skill
			case 4:
				if (numSkills != 0) return 'p';
				break;
		}
	}
	while (true);
	return ' ';
}
