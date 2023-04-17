/*****************************************************************************************
 ** Program Filename: maze.cpp
 ** Author: Crystal Lee
 ** Date: 2023-03-06
 ** Description: Implementation file for the Maze class. 
 ** Input: Depends on the function, but generally nothing.
 ** Output: Depends on the function, but it generally gives out information about the 
 **			class. Some of them also do some other functions for changing it.
 ****************************************************************************************/
#include<math.h>
#include<vector>
#include<random>

#include "consts.h"
#include "maze.h"
#include "mazelocation.h"
#include "openspace.h"
#include "wall.h"

#include "mazeperson.h"
#include "ai.h"
#include "intrepidstudent.h"
#include "instructor.h"
#include "ta.h"

#include "optionspanel.h"

/*****************************************************************************************
 ** Function: Maze (constructor)
 ** Description: Makes a new square Maze object.
 ** Parameters: size (int) - side length of the maze (square) 
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
Maze::Maze(int size) : Maze(size, size) {}

/*****************************************************************************************
 ** Function: Maze (constructor)
 ** Description: Makes a new Maze object.
 ** Parameters: width (int) - width of the new Maze
 **				height (int) - height of the new Maze
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
Maze::Maze(int width, int height) {
	TA::setAppeased(false);

	// init map
	mazeMap = std::vector<std::vector<MazeLocation*>>(height);
	for (int i = 0; i < height; i++) mazeMap[i] = std::vector<MazeLocation*>(width);

	OptionsPanel::loadFromFile(difficulty, useFogOfWar, useAI);
	initMap();
	skillCheck();
}

/*****************************************************************************************
 ** Function: ~Maze (destructor)
 ** Description: Deletes the current Maze object.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
Maze::~Maze() {
	// delete all spaces
	for (int i = 0; i < mazeMap.size(); i++)
		for (int j = 0; j < mazeMap[i].size(); j++)
			if (mazeMap[i][j] != nullptr) delete mazeMap[i][j];

	// delete all people 
	for (int i = 0; i < people.size(); i++) 
		if (people[i] != nullptr) delete people[i];
}

/*****************************************************************************************
 ** Function: initMap
 ** Description: Initializes the map for the maze (walls, people, and skills).
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Maze::initMap() {
	do {
		initWalls();
		initPeople();
		initSkills();
	} while(!validMap());
}

/*****************************************************************************************
 ** Function: initWalls
 ** Description: Initializes the walls for the maze.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Maze::initWalls() {
	for (int i = 0; i < mazeMap.size(); i++) {
		for (int j = 0; j < mazeMap[0].size(); j++) {
			if (mazeMap[i][j] != nullptr) delete mazeMap[i][j];
			mazeMap[i][j] = new OpenSpace();
		}
	}
	
	int row, col;
	int numWalls = MAZE_MIN_WALLS;
	numWalls += (mazeMap.size()*mazeMap[0].size() - MAZE_SCALE)*MAZE_WALL_PER_SCALE/MAZE_SCALE;
	numWalls *= 1 + difficulty*MAZE_DIFF_MULT_WALL;		// difficulty multiplier

	for (int i = 0; i < numWalls; i++) {
		do {
			row = rand() % mazeMap.size();
			col = rand() % mazeMap[0].size();
		} while (!mazeMap[row][col]->isOccupiable());

		delete mazeMap[row][col];
		mazeMap[row][col] = new Wall();
	}
}

/*****************************************************************************************
 ** Function: initSkills
 ** Description: Initializes the skills for the maze. Doesn't put them on the instructor.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Maze::initSkills() {
	int row, col; 
	int numSkills = MAZE_MIN_SKILLS;
	numSkills += (mazeMap.size()*mazeMap[0].size() - MAZE_SCALE)*MAZE_SKILLS_PER_SCALE/MAZE_SCALE;
	numSkills *= 1 + difficulty*MAZE_DIFF_MULT_SKILLS;		// difficulty multiplier
	
	for (int i = 0; i < numSkills; i++) {
		do {
			row = rand() % mazeMap.size();
			col = rand() % mazeMap[0].size();
		} while (!mazeMap[row][col]->isOccupiable() || 
				dynamic_cast<OpenSpace*>(mazeMap[row][col])->getHasSkill() || 
				(row == people[1]->getRow() && col == people[1]->getCol()));
		
		dynamic_cast<OpenSpace*>(mazeMap[row][col])->setHasSkill(true);
	}
}

/*****************************************************************************************
 ** Function: initPeople
 ** Description: Initializes all of the people in the maze.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: The student isn't right next to an Instructor or TA.
 ****************************************************************************************/
void Maze::initPeople() {
	if (people.size() != 0) {
		for (int i = 0; i < people.size(); i++) 
			if (people[i] != nullptr) delete people[i];
		people.clear();
	}

	MazePerson* p;

	if (useAI) p = new AI();
	else p = new IntrepidStudent();
	placeRandomly(p, false, false);
	
	p = new Instructor();
	placeRandomly(p, true, false);

	initTAs();
}

/*****************************************************************************************
 ** Function: initTAs
 ** Description: Initializes all of the TAs in the maze.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: No TA is next to the student.
 ****************************************************************************************/
void Maze::initTAs() {
	int numTAs = MAZE_MIN_TAS;
	numTAs += (mazeMap.size()*mazeMap[0].size() - MAZE_SCALE)*MAZE_TA_PER_SCALE/MAZE_SCALE;
	numTAs *= 1 + difficulty*MAZE_DIFF_MULT_TA;		// difficulty multiplier
	
	MazePerson* p;
	for (int i = 0; i < numTAs; i++) {
		p = new TA();
		placeRandomly(p, true, true);
	}
}

/*****************************************************************************************
 ** Function: placeRandomly
 ** Description: Randomly places the given person inside the maze, checking for the 
 **				 student, instructor, or both based on the given inputs.
 ** Parameters: p (MazePerson*) - the person to place
 **				checkPlayer (bool) - whether to check for being adjacent to the player 
 **				checkInstructor (bool) - whether to check for being in the same square as
 **					the instructor or not
 ** Pre-Conditions: none.
 ** Post-Conditions: The given person has been randomly placed into the grid.
 ****************************************************************************************/
void Maze::placeRandomly(MazePerson* p, bool checkPlayer, bool checkInstructor) {
	int row, col;
	do {
		row = rand() % mazeMap.size();
		col = rand() % mazeMap[0].size();
	} while (!mazeMap[row][col]->isOccupiable() 
			 || (checkPlayer && adjacentToPlayer(row, col))
			 || (checkInstructor && row == people[1]->getRow() && row == people[1]->getCol()));
	p->setLocation(row, col);
	people.push_back(p);
}

/*****************************************************************************************
 ** Function: validMap
 ** Description: Checks whether the current map is valid.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
bool Maze::validMap() const {
	std::vector<int> rows, cols;
	int numSkills = 0;
	bool instructorReachable = false;

	validMapHelper(rows, cols, numSkills, instructorReachable, 
				   people[0]->getRow(), people[0]->getCol());
	return instructorReachable && numSkills >= 3;
}

/*****************************************************************************************
 ** Function: validMap
 ** Description: Checks whether the current map is valid.
 ** Parameters: rows (vector<int>&) - rows of checked cells (corresponds to cols)
 **				cols (vector<int>&) - columns of checked cells (corresponds to rows)
 **				numSkills (int&) - current number of skills found
 **				instructorReachable (bool&) - whether we've found the instructor yet
 **				row (int) - the current row to check
 **				col (int) - the current column to check
 ** Pre-Conditions: none.
 ** Post-Conditions: Gives how many skills were found and whether the instructor could be
 **					 reached or not. 
 ****************************************************************************************/
void Maze::validMapHelper(std::vector<int>& rows, std::vector<int>& cols, int& numSkills, 
					bool& instructorReachable, int row, int col) const {
	// check that the student can do two things: reach the instructor and get 3 skills
	// slow as fuck but don't think there's any other way to do this
	
	// is it a valid space to check 
	if (row < 0 || row >= mazeMap.size() || col < 0 || col >= mazeMap[0].size() 
		|| !mazeMap[row][col]->isOccupiable()) 
		return;
	for (int i = 0; i < rows.size(); i++) 
		if (row == rows[i] && col == cols[i]) return;

	// add to checked spaces 
	rows.push_back(row);
	cols.push_back(col);

	// increment num skills if possible
	if (((OpenSpace*)mazeMap[row][col])->getHasSkill()) numSkills++;

	// set instructorReachable if possible
	instructorReachable |= people[1]->getRow() == row && people[1]->getCol() == col;

	if (instructorReachable && numSkills >= 3) return;
	
	// recurse with cardinal dirs
	validMapHelper(rows, cols, numSkills, instructorReachable, row - 1, col);
	validMapHelper(rows, cols, numSkills, instructorReachable, row + 1, col);
	validMapHelper(rows, cols, numSkills, instructorReachable, row, col - 1);
	validMapHelper(rows, cols, numSkills, instructorReachable, row, col + 1);
}

/*****************************************************************************************
 ** Function: adjacentToPlayer
 ** Description: Checks whether the given person is next to the player.
 ** Parameters: p (MazePerson*) - the person to check adjacency with
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
bool Maze::adjacentToPlayer(MazePerson* p) const {
	return adjacent(p->getRow(), p->getCol(), people[0]->getRow(), people[0]->getCol());
}

/*****************************************************************************************
 ** Function: adjacentToPlayer
 ** Description: Checks whether the given square is next to the player.
 ** Parameters: row (int) - the row of the square to check
 **				col (int) - the col of the square to check 
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
bool Maze::adjacentToPlayer(int row, int col) const {
	return adjacent(row, col, people[0]->getRow(), people[0]->getCol());
}

/*****************************************************************************************
 ** Function: adjacent
 ** Description: Checks whether the two squares are adjacent.
 ** Parameters: row1 (int) - the row of the first square to check
 **				col1 (int) - the col of the first square to check 
 **				row2 (int) - the row of the second square to check
 **				col2 (int) - the col of the second square to check 
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
bool Maze::adjacent(int row1, int col1, int row2, int col2) const {
	return (row1 == row2 && std::abs(col1 - col2) < 2) || 
			(col1 == col2 && std::abs(row1 - row2) < 2);
}

/*****************************************************************************************
 ** Function: getLoc
 ** Description: Gets the MazeLocation at the given row and column.
 ** Parameters: row (int) - the row of the square to get
 **				col (int) - the col of the square to get 
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
const MazeLocation* Maze::getLoc(int row, int col) const {
	if (row < 0 || row >= mazeMap.size() || col < 0 || col >= mazeMap[row].size())
		return nullptr;
	return mazeMap[row][col];
}

/*****************************************************************************************
 ** Function: getPerson
 ** Description: Gets the MazeLocation at the given row and column.
 ** Parameters: row (int) - the row of the square to get
 **				col (int) - the col of the square to get 
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
const MazePerson* Maze::getPerson(int index) const {
	if (index < 0 || index > people.size()) return nullptr;
	return people[index];
}

/*****************************************************************************************
 ** Function: getStudent
 ** Description: Gets the player.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
const IntrepidStudent* Maze::getStudent() const {
	return dynamic_cast<IntrepidStudent*>(people[0]);
}

/*****************************************************************************************
 ** Function: getNumSkills
 ** Description: Gets the number of skills the player has right now.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int Maze::getNumSkills() const {
	if (people.size() < 1) return -1;
	return dynamic_cast<IntrepidStudent*>(people[0])->getNumSkills();	
}

/*****************************************************************************************
 ** Function: getUsingAI
 ** Description: Gets whether the current player is an AI or not.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
bool Maze::getUsingAI() const { return useAI; }

/*****************************************************************************************
 ** Function: getUsingFog
 ** Description: Gets whether fog of war is currently active.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
bool Maze::getUsingFog() const { return useFogOfWar; }

/*****************************************************************************************
 ** Function: getMazeHeight
 ** Description: Gets the number of rows in the maze (its height).
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int Maze::getMazeHeight() const { return mazeMap.size(); }

/*****************************************************************************************
 ** Function: getMazeWidth
 ** Description: Gets the number of columns in the maze (its width).
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int Maze::getMazeWidth() const { return mazeMap[0].size(); }

/*****************************************************************************************
 ** Function: getMazeHeight
 ** Description: Gets the number of rows in the maze (its height).
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
int Maze::getNumPeople() const { return people.size(); }

/*****************************************************************************************
 ** Function: checkGameEnd
 ** Description: Checks whether the maze shows tha the game has ended or not. 
 ** Parameters: won (bool&) - output param for whether the player won or lst
 ** Pre-Conditions: none.
 ** Post-Conditions: Returns whether the game has ended or not. If it has, then won is set
 **					 to whether the user won or not. If this returns false, there are no 
 **					 guarantees.
 ****************************************************************************************/
bool Maze::checkGameEnd(bool& won) const {
	// instructor check 
	if (adjacentToPlayer(people[1])) {
		won = dynamic_cast<IntrepidStudent*>(people[0])->getNumSkills() >= 3;
		return true;
	}

	// TA check
	if (TA::getAppeased()) return false;
	for (int i = 2; i < people.size(); i++) {
		if (adjacentToPlayer(people[i])) {
			won = false;
			return true;
		}
	}

	return false;
}

/*****************************************************************************************
 ** Function: appeaseTAs
 ** Description: Appeases the TAs if possible.
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Maze::appeaseTAs() {
	IntrepidStudent* student = dynamic_cast<IntrepidStudent*>(people[0]);

	if (student->getNumSkills() > 0) student->useSkill();
}

/*****************************************************************************************
 ** Function: takeTurn
 ** Description: Takes a turn. 
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Maze::takeTurn(int input) {
	TA::decAppeased();
	
	if (useAI) input = dynamic_cast<AI*>(people[0])->getChar(mazeMap, people);
	
	// give input to player, check for skills
	if (input == 'p') appeaseTAs();
	movePerson(people[0], dynamic_cast<IntrepidStudent*>(people[0])->move(input));
	skillCheck();

	// loop through the rest and make them move 
	for (int i = 1; i < people.size(); i++) movePerson(people[i], people[i]->move());
}

/*****************************************************************************************
 ** Function: movePerson
 ** Description: Moves a single person. 
 ** Parameters: p (MazePerson*) - the person to move
 **				move (int) - the move to use 
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
bool Maze::movePerson(MazePerson* p, int move) {
	int r = p->getRow(), c = p->getCol();
	bool moved = false;

	switch(move) {
		case MAZE_MOVE_NONE:
			return false;
		case MAZE_MOVE_UP:
			if (r > 0) {
				r--;
				moved = true;
			}
			break;
		case MAZE_MOVE_DOWN:
			if (r < mazeMap.size() - 1) {
				r++;
				moved = true;
			}
			break;
		case MAZE_MOVE_LEFT:
			if (c > 0) {
				c--;
				moved = true;
			}
			break;
		case MAZE_MOVE_RIGHT:
			if (c < mazeMap[0].size() - 1) {
				c++;
				moved = true;
			}
			break;
	}
	// gotta check if the space itself is ok (also can't have an instructor)
	if (moved && mazeMap[r][c]->isOccupiable() 
		&& (r != people[1]->getRow() || c != people[1]->getCol())) {
		p->setLocation(r, c);
		return true;
	}
	return false;
}

/*****************************************************************************************
 ** Function: skillCheck
 ** Description: Checks if the player is on a skill. If so, the player collects the skill.  
 ** Parameters: none.
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
void Maze::skillCheck() {
	IntrepidStudent* stu = dynamic_cast<IntrepidStudent*>(people[0]);
	OpenSpace* sp = dynamic_cast<OpenSpace*>(mazeMap[stu->getRow()][stu->getCol()]);
	if (sp->getHasSkill()) {
		stu->addSkill();
		sp->setHasSkill(false);
	}
}

/*****************************************************************************************
 ** Function: getInFog
 ** Description: Checks if the person at the given index is in fog. 
 ** Parameters: personIndex (int) - index of the person to check 
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
bool Maze::getInFog(int personIndex) const {
	if (!useFogOfWar) return false;

	int stuRow = people[0]->getRow(), stuCol = people[0]->getCol();
	int row = people[personIndex]->getRow(), col = people[personIndex]->getCol();

	return std::abs(stuRow - row) > MAZE_FOG_SIZE || std::abs(stuCol - col) > MAZE_FOG_SIZE;
}

/*****************************************************************************************
 ** Function: getInFog
 ** Description: Checks if the given square is in fog.
 ** Parameters: row (int) - the row of the square to check
 **				col (int) - the column of the square to check
 ** Pre-Conditions: none.
 ** Post-Conditions: none.
 ****************************************************************************************/
bool Maze::getInFog(int row, int col) const {
	if (!useFogOfWar) return false;

	int stuRow = people[0]->getRow(), stuCol = people[0]->getCol();
	return std::abs(stuRow - row) > MAZE_FOG_SIZE || std::abs(stuCol - col) > MAZE_FOG_SIZE;
}
