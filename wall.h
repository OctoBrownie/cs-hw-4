/*****************************************************************************************
 ** Program Filename: wall.h
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Header file for the Wall class, which represents an unoccupiable space in
 **				 the maze. 
 ** Input: Depends on the function, but usually nothing.
 ** Output: Depends on the function, but generally gets information about the class.
 ****************************************************************************************/
#ifndef __wall_h__
#define __wall_h__

#include "mazelocation.h"

class Wall : public MazeLocation {
	public:
		virtual bool isOccupiable() const override;
		virtual int getDisplayChar(bool inFog) const override;
};

#endif 