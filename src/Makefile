CXX = g++
CXXFLAGS = -std=c++14 -g -Wall -MMD -Werror=vla
EXEC = ctor
OBJECTS = main.o Board.o BoardDisplay.o Dice.o Edge.o Player.o Sideline.o SourceData.o Tile.o UserDisplay.o Vertex.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
