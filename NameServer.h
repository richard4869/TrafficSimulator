#pragma once

#include "Printer.h"

_Task VendingMachine;

_Task NameServer {
	Printer &printer;
	unsigned int numVendingMachines;
	unsigned int numStudents;
	unsigned int index;
	VendingMachine **machine_list;                                     //list of machines
	int *student_assignment;                                           //current assigned machine for each student
    void main();
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
};
