#include "Student.h"
#include "VendingMachine.h"
#include "MPRNG.h"
#include "WATCard.h"
//#include <iostream>
extern MPRNG mprng;

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff, unsigned int id, unsigned int maxPurchases ):
	printer(prt),
	nameServer(nameServer),
	cardOffice(cardOffice),
	groupoff(groupoff),
	id(id),
	maxPurchases(maxPurchases),
	giftcard_used(false){}

Student::~Student(){
	printer.print(Printer::Kind::Student, id, 'F');
}

void Student::main(){
	int num_purchasing = mprng(1,maxPurchases);
	VendingMachine::Flavours favourite_flavour = (VendingMachine::Flavours)mprng(3);

	printer.print(Printer::Kind::Student, id, 'S', (int)favourite_flavour, num_purchasing);
	//get cards
	WATCard::FWATCard watcard = cardOffice.create(id, 5);
	WATCard::FWATCard giftcard = groupoff.giftCard();

	int num_purchased = 0;
	VendingMachine *machine = nameServer.getMachine(id);
	printer.print(Printer::Kind::Student, id, 'V', machine->getId());
	while(num_purchased < num_purchasing){
		try{
			_Select(watcard || giftcard){										//wither watcard or giftcard is available
				if (watcard.available()){										//watcard is available
					WATCard *physical_card = watcard();							//interesting
					for(;;){
						try{
							yield(mprng(1, 10));
							machine->buy(favourite_flavour, *physical_card);	//buy
							printer.print(Printer::Kind::Student, id, 'B', (int)favourite_flavour, physical_card->getBalance());
							break;
						}catch(VendingMachine::Funds e){						//not enough fund
							try{
								watcard = cardOffice.transfer(id, machine->cost() + 5, physical_card);
							}catch(WATCardOffice::Lost e){
								printer.print(Printer::Kind::Student, id, 'L');
								watcard.reset();
								watcard = cardOffice.create(id, 5);
								break;
							}
						}catch(VendingMachine::Stock e){
							machine = nameServer.getMachine(id);
							printer.print(Printer::Kind::Student, id, 'V', machine->getId());
						}catch(VendingMachine::Free e){
							printer.print(Printer::Kind::Student, id, 'A', (int)favourite_flavour, physical_card->getBalance());
							yield(4);											//watch ads
							break;
						}
					}
				}else if(giftcard.available()){									//giftcard is available
					for(;;){
						try{
							yield(mprng(1, 10));
							machine->buy(favourite_flavour, *giftcard());	//buy
							printer.print(Printer::Kind::Student, id, 'G', (int)favourite_flavour, giftcard()->getBalance());
							//std::cout<<"before delete"<<std::endl;
							delete giftcard();
							giftcard.reset();
							giftcard_used = true;
							break;
						}catch(VendingMachine::Stock e){
							machine = nameServer.getMachine(id);				//find a new machine
							printer.print(Printer::Kind::Student, id, 'V', machine->getId());
						}catch(VendingMachine::Free e){
							printer.print(Printer::Kind::Student, id, 'a', (int)favourite_flavour, giftcard()->getBalance());
							yield(4);											//watch ads
							break;
						}
					}
				}
			}
			num_purchased++;
		}catch(WATCardOffice::Lost e){											//Lost
			printer.print(Printer::Kind::Student, id, 'L');
			watcard.reset();
			watcard = cardOffice.create(id, 5);									//get a new one
		}
	}

	_Select(watcard){
		try{
			WATCard *physical_card = watcard();
			delete physical_card;
		}catch(WATCardOffice::Lost e){

		}
	}
	if(!giftcard_used){
		WATCard *physical_card = giftcard();
		giftcard.reset();
		delete physical_card;
	}
}