#pragma once

#include "VendingMachine.h"
#include "Printer.h"
#include "NameServer.h"

_Task BottlingPlant;

_Task Truck {
	Printer &printer;
	NameServer &nameServer;
	BottlingPlant &plant;
    unsigned int numVendingMachines;
    unsigned int maxStockPerFlavour;
    unsigned int cargo[VendingMachine::Flavours::Num];
    void main();
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};