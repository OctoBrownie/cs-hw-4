/*****************************************************************************************
 ** Program Filename: instructor.h
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Header file for the Instructor class, representing the "final boss" of a
 **				 maze.
 ** Input: For all functions here, none.
 ** Output: Depends on the function, but it generally gives out information about the 
 **			class. 
 ****************************************************************************************/
#ifndef __instructor_h__
#define __instructor_h__

#include "mazeperson.h"

class Instructor : public MazePerson {
	public:
		virtual int move() const override;
		virtual int getDisplayChar() const override;
};

#endif 