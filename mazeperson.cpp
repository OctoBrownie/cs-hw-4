/*****************************************************************************************
 ** Program Filename: mazeperson.cpp
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Implementation file for the MazePerson class.
 ** Input: Depends on the function, but usually nothing.
 ** Output: Depends on the function, but it generally gives out information about the 
 **			class.
 ****************************************************************************************/
#include "mazeperson.h"

/*****************************************************************************************
 ** Function: setLocation
 ** Description: Sets the location of the person.
 ** Parameters: newRow (int) - the person's new row 
 **				newCol (int) - the person's new column
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void MazePerson::setLocation(int newRow, int newCol) {
	row = newRow;
	col = newCol;
}

/*****************************************************************************************
 ** Function: getRow
 ** Description: Gets the current row in the maze of the person.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int MazePerson::getRow() const { return row; }

/*****************************************************************************************
 ** Function: getCol
 ** Description: Gets the current column in the maze of the person.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int MazePerson::getCol() const { return col; }
