# Makefile for TankGame

#-----------------------------------------------------------------------

# GNU C/C++ compiler and linker:
CC = gcc
CXX = g++
LINK = g++

# Turn on optimization and warnings (add -g for debugging with gdb):
CFLAGS = -O -Wall
CXXFLAGS = -O -Wall

#-----------------------------------------------------------------------

default: b17.cpp readIn.cpp process.cpp globals.h
	g++ --std=c++11 b17.cpp readIn.cpp process.cpp -o b17

# utility targets
clean:
	@rm -f *.o *~ core
	@rm b17