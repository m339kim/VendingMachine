CXX = u++					# compiler
CXXFLAGS = -g -multi -O2 -Wall -Wextra -MMD # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = bank.o bottlingPlant.o config.o groupoff.o main.o nameServer.o parent.o printer.o student.o truck.o \
	vendingMachine.o WATCard.o WATCardOffice.o
EXEC = soda

# TESTS

# TEST_OBJECTS

DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"

#############################################################

.PHONY : all clean test

all : ${EXEC}									# build all executables


${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################
${OBJECTS} : ${MAKEFILE_NAME}					# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}								# include *.d files containing program dependences

clean :											# remove files that can be regenerated
	rm -f *.d *.o ${EXEC}
