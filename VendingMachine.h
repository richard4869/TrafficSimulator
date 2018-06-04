#pragma once
#include "Printer.h"
#include "NameServer.h"
#include "WATCard.h"

_Task VendingMachine {
    Printer &printer;
    NameServer &nameServer;
    unsigned int id;
    unsigned int sodaCost;
    unsigned int maxStockPerFlavour;
    bool restocking;
    unsigned int *stock_info;
    void main();
  public:
    enum Flavours { Blues_Black_Cherry,                             // flavours of soda (YOU DEFINE)
                    Classic_Cream_Soda, 
                    Rock_Root_Beer, 
                    Jazz_Lime,
                    Num };

    _Event Free {};                           // free, advertisement
    _Event Funds {};                          // insufficient funds
    _Event Stock {};                          // out of stock for particular flavour

    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour );
    ~VendingMachine();
    void buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
};
