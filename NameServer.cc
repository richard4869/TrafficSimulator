#include "NameServer.h"
#include "VendingMachine.h"
//#include <iostream>
NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ):
	printer(prt),
	numVendingMachines(numVendingMachines),
	numStudents(numStudents),
	index(0){
		machine_list = new VendingMachine *[numVendingMachines];
		student_assignment = new int [numStudents];
		for (unsigned int i = 0; i < numStudents; i++){
			student_assignment[i] = i % numVendingMachines;
		}
	}

NameServer::~NameServer(){
	delete[] machine_list;
	delete[] student_assignment;
	printer.print(Printer::Kind::NameServer, 'F');
}

void NameServer::VMregister( VendingMachine *vendingmachine ){
	printer.print(Printer::Kind::NameServer, 'R', vendingmachine->getId());
	machine_list[index] = vendingmachine;
	index++;
}

VendingMachine *NameServer::getMachine( unsigned int id ){
	int machine_assigned = student_assignment[id];
	//std::cout<<"getMachine:"<<id << " "<< machine_assigned <<" "<<numStudents<< std::endl;
	printer.print(Printer::Kind::NameServer, 'N', id, machine_list[machine_assigned]->getId());
	student_assignment[id] = (machine_assigned + 1) % numVendingMachines;							//assigned to next 
	return machine_list[machine_assigned];
}

VendingMachine **NameServer::getMachineList(){
	return machine_list;
}

void NameServer::main(){
	printer.print(Printer::Kind::NameServer, 'S');
	for(;;){
		_Accept (~NameServer) {
			break;
		}or _When(index == numVendingMachines) _Accept(getMachine, getMachineList) {				//start service

		}or _Accept(VMregister) {																	//for machine to register

		}
	}
}