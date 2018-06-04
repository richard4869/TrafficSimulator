TYPE:=EXT
OPT:=-O2

CXX = u++					# compiler
CXXFLAGS = -g -Wall ${OPT} -multi -MMD -std=c++11 -DIMPLTYPE_${TYPE} # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS2 = Bank.o BottlingPlant.o config.o Groupoff.o NameServer.o Parent.o Printer.o Soda.o Student.o Truck.o VendingMachine.o WATCard.o WATCardOffice.o
EXEC2 = soda

OBJECTS = ${OBJECTS2}				# all object files
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXECS = ${EXEC2}				# all executables

#############################################################

.PHONY : all clean

all : ${EXECS}					# build all executables

${EXEC2} : ${OBJECTS2}
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}	

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXECS}
