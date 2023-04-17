CC=g++
FLAGS=--std=c++11 -l ncurses
# enable debugging by uncommenting this, disable by commenting
DEBUG_FLAGS=-g
EXE_FILE=ass4
OBJECTS=main.cpp ncursesutils.o \
panel.o startpanel.o optionspanel.o game.o \
maze.o wall.o openspace.o \
mazeperson.o instructor.o intrepidstudent.o ta.o ai.o 

.PHONY: main
main: $(OBJECTS)
	$(CC) $(FLAGS) $(DEBUG_FLAGS) $(OBJECTS) -o $(EXE_FILE)

%.o: %.cpp %.h consts.h
	$(CC) $(FLAGS) $(DEBUG_FLAGS) -c $< 

.PHONY: clean
clean:
	rm -f *.o $(EXE_FILE)

.PHONY: clean-win
clean-win: 
	powershell "rm -Force *.o; rm -Force $(EXE_FILE).exe"

