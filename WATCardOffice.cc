#include "WATCardOffice.h"
#include <iostream>
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ):
	printer(prt),
	bank(bank),
	numCouriers(numCouriers){
		couriers = new Courier* [numCouriers];
		for(unsigned int i = 0; i < numCouriers; i++){
			couriers[i] = new Courier(printer, bank, i, this);
		}
		job_queue = queue<Job*>();
	}

WATCardOffice::~WATCardOffice(){
	for(unsigned int i = 0; i < numCouriers; i++){
		delete couriers[i];
	}
	delete[] couriers;
	printer.print(Printer::Kind::WATCardOffice, 'F');
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ){
	WATCard *watcard = new WATCard();										//new card
	Job *j = new Job(sid, amount, watcard);									//careat job
	job_queue.push(j);														//add to queue
	printer.print(Printer::Kind::WATCardOffice, 'C', sid, amount);
	return j->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ){
	Job *j = new Job(sid, amount, card);									//careat job
	job_queue.push(j);														//add to queue
	printer.print(Printer::Kind::WATCardOffice, 'T', sid, amount);
	return j->result;
}

WATCardOffice::Job *WATCardOffice::requestWork(){
	if(job_queue.empty() == true){
		return NULL;
	}
	Job *job = job_queue.front();
	job_queue.pop();
	return job;
}

void WATCardOffice::main(){
	printer.print(Printer::Kind::WATCardOffice, 'S');
	for(;;){
		_Accept(~WATCardOffice){
			//std::cout<<"office end"<<std::endl;
			for(unsigned int i = 0; i < numCouriers; i++){
				job_queue.push(NULL);
				_Accept(requestWork);
			}
    		break;
		}or _Accept(create, transfer) {
		}or _When(job_queue.size() > 0) _Accept(requestWork){
			printer.print(Printer::Kind::WATCardOffice, 'W');
    	}
	}
}