#pragma once
#include "WATCardOffice.h"
#include "Printer.h"
#include "Groupoff.h"
_Task Student {
	Printer &printer;
	NameServer &nameServer;
	WATCardOffice &cardOffice;
	Groupoff &groupoff;
	unsigned int id;
	unsigned int maxPurchases;
	bool giftcard_used;											//flag variable to check if the giftcard has been used
    void main();
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
             unsigned int id, unsigned int maxPurchases );
    ~Student();
};
