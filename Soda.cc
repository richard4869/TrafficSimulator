#include "Bank.h"
#include "BottlingPlant.h"
#include "config.h"
#include "Groupoff.h"
#include "MPRNG.h"
#include "NameServer.h"
#include "Parent.h"
#include "Printer.h"
#include "Student.h"
#include "Truck.h"
#include "VendingMachine.h"
#include "WATCard.h"
#include "WATCardOffice.h"

#include <iostream>
#include <string>

using namespace std;

MPRNG mprng;

void uMain::main() {
	string filename = "soda.config";
	int seed = getpid();
	try{
		switch ( argc ) {
			case 3: seed = atoi( argv[2] );
					if(seed < 0)throw 1;
			case 2:	filename = argv[1]; 
			case 1: break;
			default:
				throw 1;
		}
	}catch(...){
		cerr<<"Usage: ./"<< argv[0] <<" [ config-file [ random-seed (> 0) ] ]"<<endl;
		exit(-1);
	}
	
	mprng.set_seed(seed);
	//Config file
	ConfigParms parms;
	processConfigFile(filename.c_str(), parms);
	//Printer
	Printer printer(parms.numStudents, parms.numVendingMachines, parms.numCouriers);
	//Bank
	Bank bank(parms.numStudents);
	//Parent
	Parent parent(printer, bank, parms.numStudents, parms.parentalDelay);
	//WATCardOffice
	WATCardOffice *office = new WATCardOffice(printer, bank, parms.numCouriers);
	//Groupoff
	Groupoff groupoff(printer, parms.numStudents, parms.sodaCost, parms.groupoffDelay);
	//NameServer
	NameServer *nameserver = new NameServer(printer, parms.numVendingMachines, parms.numStudents);
	//VendingMachine
	VendingMachine **vending_machines = new VendingMachine*[parms.numVendingMachines];
	for(unsigned int i = 0; i < parms.numVendingMachines; i++){
		vending_machines[i] = new VendingMachine(printer, *nameserver, i, parms.sodaCost, parms.maxStockPerFlavour);
	}
	//bottling plant
	BottlingPlant *plant = new BottlingPlant(printer, 
											*nameserver, 
											parms.numVendingMachines, 
											parms.maxShippedPerFlavour, 
											parms.maxStockPerFlavour, 
											parms.timeBetweenShipments);
	//Students
	Student **students = new Student*[parms.numStudents];
	for(unsigned int i = 0; i < parms.numStudents; i++){
		students[i] = new Student(printer, *nameserver, *office, groupoff, i, parms.maxPurchases);
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//delete Students
	for(unsigned int i = 0; i < parms.numStudents; i++){
		delete students[i];
	}
	delete[] students;
	//delete bottling plant
	delete plant;
	//delete VendingMachine
	for(unsigned int i = 0; i < parms.numVendingMachines; i++){
		delete vending_machines[i];
	}
	delete[] vending_machines;
	//delete nameserver
	delete nameserver;
	//delete WATCardOffice
	delete office;
}