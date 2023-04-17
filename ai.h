/*****************************************************************************************
 ** Program Filename: ai.h
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Header file for the AI class, which plays the game instead of the user. 
 ** Input: For the only function defined here, it's the map of the entire maze plus the 
 **		   people in it.
 ** Output: For the only function defined here, it's the move the AI wants to take. 
 ****************************************************************************************/
#ifndef __ai_h__
#define __ai_h__

#include<vector>

#include "intrepidstudent.h"
#include "mazelocation.h"

class AI : public IntrepidStudent {
	public:
		int getChar(const std::vector<std::vector<MazeLocation*>>& map, 
					const std::vector<MazePerson*>& people) const;
};


#endif 