#include "VendingMachine.h"
#include "MPRNG.h"
extern MPRNG mprng;
VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour ):
	printer(prt),
	nameServer(nameServer),
	id(id),
	sodaCost(sodaCost),
	maxStockPerFlavour(maxStockPerFlavour),
	restocking(false)
	{
		stock_info = new unsigned int[Flavours::Num];
		for(int i = 0; i < Flavours::Num; i++){
			stock_info[i] = 0;
		}
		nameServer.VMregister(this);
	}

VendingMachine::~VendingMachine(){
	delete[] stock_info;
	printer.print(Printer::Kind::Vending, id, 'F');
}

void VendingMachine::buy( Flavours flavour, WATCard &card ){
	if(stock_info[(int)flavour] == 0){						//out of stock
		_Throw Stock();
	}else if(card.getBalance() < sodaCost){					//not enough money
		_Throw Funds();
	}else{
		if(mprng(4) == 0){									//free drink
			stock_info[(int)flavour] -= 1;
			_Throw Free();
		}else{												//buy
			card.withdraw(sodaCost);
			stock_info[(int)flavour] -= 1;
		}
		printer.print(Printer::Kind::Vending, id, 'B', (int)flavour, stock_info[(int)flavour]);
	}
}

unsigned int *VendingMachine::inventory(){
	restocking = true;
	return stock_info;
}

void VendingMachine::restocked(){

}

_Nomutex unsigned int VendingMachine::cost(){
	return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId(){
	return id;
}

void VendingMachine::main(){
	printer.print(Printer::Kind::Vending, id, 'S', sodaCost);
	for(;;){
		try{
			_When(!restocking) _Accept(~VendingMachine){
				break;
			}or _Accept(inventory){											//restocking start
				printer.print(Printer::Kind::Vending, id, 'r');
			}or _When(restocking) _Accept(restocked){						//restocking finish
				printer.print(Printer::Kind::Vending, id, 'R');
				restocking = false;
			}or _When(!restocking) _Accept(buy){							//buy

			}
		}catch( uMutexFailure::RendezvousFailure ){

		}
	}
}