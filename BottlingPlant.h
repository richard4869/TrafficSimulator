#pragma once
#include "Truck.h"

_Task BottlingPlant {
	Printer &printer;
	NameServer &nameServer;
	unsigned int numVendingMachines;
    unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
    unsigned int timeBetweenShipments;
    int *production;                                        //amount produced of each flavour
    bool closed;                                            //flat variable
    Truck *truck;
    void main();
    void runproduction();                                   //produce the drink process
  public:
    _Event Shutdown {};                       // shutdown plant
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    ~BottlingPlant();
    void getShipment( unsigned int cargo[] );
};