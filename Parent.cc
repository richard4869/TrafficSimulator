#include "Parent.h"
#include "MPRNG.h"
extern MPRNG mprng;
Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay ):
	printer(prt),
	bank(bank),
	numStudents(numStudents),
	parentalDelay(parentalDelay){

	}

Parent::~Parent(){
	printer.print(Printer::Kind::Parent, 'F');
}

void Parent::main(){
	printer.print(Printer::Kind::Parent, 'S');
	for(;;){
		_Accept(Parent::~Parent){
			break;
		}_Else{
			yield(parentalDelay);
			int id = mprng(numStudents - 1);
			int amount = mprng(1,3);
			printer.print(Printer::Kind::Parent, 'D', id, amount);
			bank.deposit(id, amount);
		}
	}
}