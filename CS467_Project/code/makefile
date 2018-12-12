cxx = g++
CXXFLAGS = -std=c++0x -lboost_system -lboost_filesystem -ggdb -g -Wall

item.o: item.h item.cpp
	${CXX} ${CXXFLAGS} -c item.cpp

puzzle.o: puzzle.h puzzle.cpp
	${CXX} ${CXXFLAGS} -c puzzle.cpp

feature.o: feature.h feature.cpp
	${CXX} ${CXXFLAGS} -c feature.cpp

room.o: room.h room.cpp
	${CXX} ${CXXFLAGS} -c room.cpp

player.o: room.o player.h player.cpp 
	${cxx} ${CXXFLAGS} -c player.cpp

game.o: game.h game.cpp
	${CXX} ${CXXFLAGS} -c game.cpp

parser.o: parser.h parser.cpp
	${CXX} ${CXXFLAGS} -c parser.cpp


main.o: main.cpp
	${CXX} ${CXXFLAGS} -c main.cpp

main: parser.o main.o game.o player.o room.o feature.o puzzle.o item.o 
	${CXX} ${CXXFLAGS} -o main -g  main.o game.o parser.o player.o room.o feature.o puzzle.o item.o 

all: main

remake: clean main
	
clean:
	rm -f *.o main
 	
