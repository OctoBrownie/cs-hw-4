/*****************************************************************************************
 ** Program Filename: mazelocation.h
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Header file for the MazeLocation class, which represents a single square
 **				 in the maze.
 ** Input: Depends on the function.
 ** Output: Depends on the function, but it generally gives out information about the 
 **			class.
 ****************************************************************************************/
#ifndef __mazelocation_h__
#define __mazelocation_h__

class MazeLocation {
	public:
		virtual bool isOccupiable() const = 0;
		virtual int getDisplayChar(bool inFog) const = 0;
};

#endif