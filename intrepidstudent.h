/*****************************************************************************************
 ** Program Filename: intrepidstudent.h
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Header file for the IntrepidStudent class, which represents the player
 **				 of the game. 
 ** Input: Depends on the function, but generally nothing.
 ** Output: Depends on the function, but it generally gives out information about the 
 **			class. 
 ****************************************************************************************/
#ifndef __intrepidstudent_h__
#define __intrepidstudent_h__

#include "mazeperson.h"

class IntrepidStudent : public MazePerson {
	protected: 
		int numSkills;
	public:
		IntrepidStudent();

		virtual int move() const override;
		int move(int inputChar) const;
		
		virtual int getDisplayChar() const override;
		
		int getNumSkills() const;
		void addSkill();
		void useSkill();
};

#endif 