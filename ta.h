/*****************************************************************************************
 ** Program Filename: ta.h
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Header file for the TA class, which represents an enemy in the maze that
 **				 either needs to be avoided or appeased.
 ** Input: Depends on the function, but usually nothing.
 ** Output: Depends on the function, but generally gets information about the class.
 ****************************************************************************************/
#ifndef __ta_h__
#define __ta_h__

#include<string>

#include "mazeperson.h"

class TA : public MazePerson {
	private: 
		static const std::string TEXT_APPEASED_1;
		static const std::string TEXT_APPEASED_2;
		static const std::string TEXT_NOT_APPEASED;

		static int appeasedTurns;
	public:
		static const std::string getStatus();
		static bool getAppeased();
		static void setAppeased(bool b);
		static void decAppeased();

		virtual int move() const override;
		virtual int getDisplayChar() const override;
};

#endif 