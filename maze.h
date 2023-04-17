/*****************************************************************************************
 ** Program Filename: maze.h
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Header file for the Maze class, which contains everything relating to the
 **				 maze of the game itself and the people on it.  
 ** Input: Depends on the function, but generally nothing.
 ** Output: Depends on the function, but it generally gives out information about the 
 **			class. Some of them also do some other functions for changing it.
 ****************************************************************************************/
#ifndef __maze_h__
#define __maze_h__

#include<vector>

#include "mazelocation.h"
#include "mazeperson.h"
#include "intrepidstudent.h"

class Maze {
	private:
		std::vector<std::vector<MazeLocation*>> mazeMap;
		std::vector<MazePerson*> people;

		int difficulty;
		bool useAI, useFogOfWar;

		void initMap();
		void initWalls();
		void initSkills();
		void initPeople();
		void initTAs();

		bool validMap() const;
		void validMapHelper(std::vector<int>& rows, std::vector<int>& cols, int& numSkills, 
								  bool& instructorReachable, int row, int col) const;
		bool adjacentToPlayer(MazePerson* p) const;
		bool adjacentToPlayer(int row, int col) const;
		bool adjacent(int row1, int col1, int row2, int col2) const;

		void placeRandomly(MazePerson* p, bool checkPlayer, bool checkInstructor);

		void appeaseTAs();
		void skillCheck();
		bool movePerson(MazePerson* p, int move);
	public:
		Maze(int size);
		Maze(int width, int height);
		~Maze();
		// no copy constructor or AOO needed (only one used at a time, always pass by ref)

		const MazeLocation* getLoc(int row, int col) const;
		
		const MazePerson* getPerson(int index) const;
		const IntrepidStudent* getStudent() const;
		int getNumPeople() const;

		int getNumSkills() const;
		bool getUsingAI() const;
		bool getUsingFog() const;

		bool getInFog(int personIndex) const;
		bool getInFog(int row, int col) const;

		int getMazeHeight() const;
		int getMazeWidth() const;

		void takeTurn(int input);

		bool checkGameEnd(bool& won) const;
};

#endif 