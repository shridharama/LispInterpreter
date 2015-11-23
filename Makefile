# This is a very simple makefile for building the Lisp interpreter
# project when using C++ on stdsun. Feel free to add any improvements:
# e.g. pattern rules, automatic tracking of dependencies, etc. There
# is a lot of info about "make" on the web.
#test
# C++ compiler
CXX = g++

# C++ compiler flags
CXXFLAGS = -g -Wall

# Creating a .o file
COMPILE = $(CXX) $(CXXFLAGS) -c

# Name of the executable; should match the contents of Runfile
EXE = myinterpreter

# All object files
OBJS = main.o scanner.o evaluator.o parser.o

# The first target is the one that is executed when you invoke
# "make". The line describing the action starts with <TAB>. Variable
# "$@" stands for the current target. 

$(EXE) : $(OBJS) # depends on all object files
	$(CXX) $^ -o $@

# An object file is dependent on the corresponding source file

%.o : %.cc
	$(COMPILE) -o $@ $<

main.o : common.h parser.h scanner.h evaluator.h

parser.o : common.h parser.h scanner.h evaluator.h

scanner.o : common.h scanner.h evaluator.h

