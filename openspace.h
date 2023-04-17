/*****************************************************************************************
 ** Program Filename: openspace.h
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Header file for the OpenSpace class, which represents an occupiable space
 **				 with a potential to hold a programming skill.
 ** Input: Depends on the function, but usually nothing.
 ** Output: Depends on the function, but it generally gives out information about the 
 **			class.
 ****************************************************************************************/
#ifndef __openspace_h__
#define __openspace_h__

#include "mazelocation.h"

class OpenSpace : public MazeLocation {
	private:
		bool hasSkill;
	public:
		OpenSpace();
		
		virtual bool isOccupiable() const override;
		virtual int getDisplayChar(bool inFog) const override;

		bool getHasSkill() const;
		void setHasSkill(bool b);
};

#endif 