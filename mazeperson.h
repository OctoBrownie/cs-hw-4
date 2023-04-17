/*****************************************************************************************
 ** Program Filename: mazeperson.h
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Header file for the MazePerson class, which represents a general person
 **				 in the maze.
 ** Input: Depends on the function, but usually nothing.
 ** Output: Depends on the function, but it generally gives out information about the 
 **			class.
 ****************************************************************************************/
#ifndef __mazeperson_h__
#define __mazeperson_h__

class MazePerson {
	protected: 
		int row, col;
	public:
		virtual int move() const = 0;
		virtual int getDisplayChar() const = 0;

		void setLocation(int row, int col);

		int getRow() const;
		int getCol() const;
};

#endif 