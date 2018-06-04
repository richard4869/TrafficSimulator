#include "BottlingPlant.h"
#include "VendingMachine.h"
#include "MPRNG.h"
#include <iostream>
extern MPRNG mprng;

BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments ):
	printer(prt),
	nameServer(nameServer),
	numVendingMachines(numVendingMachines),
	maxShippedPerFlavour(maxShippedPerFlavour),
	maxStockPerFlavour(maxStockPerFlavour),
	timeBetweenShipments(timeBetweenShipments),
	closed(false),
	truck(NULL){
		production = new int [VendingMachine::Flavours::Num];
		for(int i = 0; i < VendingMachine::Flavours::Num; i++){
			production[i] = 0;
		}
	}

BottlingPlant::~BottlingPlant(){
	delete[] production;
	if (truck != NULL){
    	delete truck; 
	}
}

void BottlingPlant::runproduction(){
	int count = 0;
	yield(timeBetweenShipments);
		for(int i = 0; i < VendingMachine::Flavours::Num; i++){
			production[i] = mprng(maxShippedPerFlavour);
			count += production[i];
		}
	printer.print(Printer::Kind::BottlingPlant, 'G', count);
}

void BottlingPlant::getShipment( unsigned int cargo[] ){
	if(closed){
		_Throw Shutdown();												//to truck
	}
	for(int i = 0; i < VendingMachine::Flavours::Num; i++){
		cargo[i] = production[i];
		production[i] = 0;
	}
	printer.print(Printer::Kind::BottlingPlant, 'P');
}

void BottlingPlant::main(){
	printer.print(Printer::Kind::BottlingPlant, 'S');
	truck = new Truck(printer, nameServer, *this, numVendingMachines, maxStockPerFlavour); 

	runproduction();
	start: for(;;){
		try{
			_Accept(~BottlingPlant){
				closed = true;
				try{
					_Accept(getShipment);								//get last shipment
				}catch( uMutexFailure::RendezvousFailure ){}
				//std::cout<<"accept end"<<std::endl;
				printer.print(Printer::Kind::BottlingPlant, 'F');
				break start;
			}or _When(!closed) _Accept(getShipment){					//normal shipment
				
				runproduction();
				yield(timeBetweenShipments);
			}
		}catch( uMutexFailure::RendezvousFailure ){

		}
	}
}
