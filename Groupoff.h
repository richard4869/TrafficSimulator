#pragma once
#include "Printer.h"
#include <vector>
#include "WATCard.h"
#include "MPRNG.h"
using namespace std;

_Task Groupoff {
	Printer &printer;
	unsigned int numStudents;
	unsigned int sodaCost;
	unsigned int groupoffDelay;
	vector<WATCard::FWATCard *> *unused_gift_cards;								//list of unused cards
	vector<WATCard::FWATCard *> *used_gift_cards;								//list of used cards
    void main();
  public:
    Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    ~Groupoff();
    WATCard::FWATCard giftCard();
};
