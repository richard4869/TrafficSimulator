#include "Groupoff.h"
#include <iostream>
extern MPRNG mprng;
Groupoff::Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ):
	printer(prt),
	numStudents(numStudents),
	sodaCost(sodaCost),
	groupoffDelay(groupoffDelay){
		unused_gift_cards = new vector<WATCard::FWATCard *>();
		used_gift_cards = new vector<WATCard::FWATCard *>();
		for (unsigned int i = 0; i < numStudents; i++) {
			unused_gift_cards->push_back(new WATCard::FWATCard());
    	}
	}

Groupoff::~Groupoff(){
	for (size_t i=0; i < unused_gift_cards->size(); ++i) {
		delete unused_gift_cards->at(i);
	}
	for (size_t i=0; i < used_gift_cards->size(); ++i) {
		delete used_gift_cards->at(i);
	}
	delete unused_gift_cards;
	delete used_gift_cards;
}

void Groupoff::main(){
	printer.print(Printer::Kind::Groupoff, 'S');
	for (unsigned int i = 0; i < numStudents;){
		_Accept(~Groupoff) {
			break;
		} or _Accept(giftCard) {
			//std::cout<<"yield"<<std::endl;
			yield(groupoffDelay);
			WATCard *card = new WATCard();									//the real one
			card->deposit(sodaCost);
			printer.print(Printer::Kind::Groupoff, 'D', sodaCost);
			used_gift_cards->back()->delivery(card);						//delivery the future
			i++;
		} _Else {

		}
	}
	printer.print(Printer::Kind::Groupoff, 'F');
}

WATCard::FWATCard Groupoff::giftCard(){
	//std::cout<<"giftCard"<<std::endl;
	int i = mprng(unused_gift_cards->size() - 1);							//random choose one
	WATCard::FWATCard *card = unused_gift_cards->at(i);
	unused_gift_cards->erase(unused_gift_cards->begin() + i);
	//std::cout<<"erase"<<std::endl;
	used_gift_cards->push_back(card);										//move to the used ones
	return *card;															//return future
}

