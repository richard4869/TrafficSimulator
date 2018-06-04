#include "Printer.h"
#include <iostream>

using namespace std;

Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ):
	numStudents(numStudents),
	numVendingMachines(numVendingMachines),
	numCouriers(numCouriers){
		numObjects = 6 + numStudents + numVendingMachines + numCouriers;
		buffer = new info* [numObjects];
		cout<< "Parent\t" << "Gropoff\t" << "WATOff\t" << "Names\t" << "Truck\t" << "Plant\t";
		for(unsigned int i = 0; i < numStudents; i++){
			cout<< "Stud" << i << "\t";
		}
		for(unsigned int i = 0; i < numVendingMachines; i++){
			cout<< "Mach" << i << "\t";
		}
		for(unsigned int i = 0; i < numCouriers; i++){
			cout<< "Cour" << i << "\t";
		}
		cout << endl;

		for(unsigned int i = 0; i < numObjects; i++){
			cout<< "*******" << "\t";
			buffer[i] = NULL;						//set buffer
		}
		cout << endl;
	}

Printer::~Printer(){
	flush();
	cout<< "***********************" << endl;
	delete[] buffer;
}

void Printer::flush(){
	for (unsigned int i = 0; i < numObjects; i++){
        if (buffer[i] != NULL) {
            printinfo(buffer[i]);
            delete buffer[i];
            buffer[i] = NULL;
        }
        cout << "\t";
    }
    cout << endl;
}

void Printer::printinfo(info* i){
	cout<< i->state;
	if(i->value1 != -1){
		cout<< i->value1;
		if(i->value2 != -1){
			cout<<",";
			cout<< i->value2;
		}
	}
}

int Printer::getindex( Kind kind, unsigned int lid ){
	int index;
	switch (kind) {
    case Printer::Kind::Student:
      index = 6 + lid;
      break;
    case Printer::Kind::Vending:
      index = 6 + numStudents + lid;
      break;
    case Printer::Kind::Courier: 
      index = 6 + numStudents + numVendingMachines + lid;
      break;
    default:
      index = -1;
      cerr<< "error: getindex" <<endl;
	}
	return index;
}

void Printer::print( Kind kind, char state ){
	if(buffer[kind] != NULL){
		flush();
	}
	buffer[kind] = new info();
	buffer[kind]->state = state;
}

void Printer::print( Kind kind, char state, int value1 ){
	if(buffer[kind] != NULL){
		flush();
	}
	buffer[kind] = new info();
	buffer[kind]->state = state;
	buffer[kind]->value1 = value1;
}

void Printer::print( Kind kind, char state, int value1, int value2 ){
	if(buffer[kind] != NULL){
		flush();
	}
	buffer[kind] = new info();
	buffer[kind]->state = state;
	buffer[kind]->value1 = value1;
	buffer[kind]->value2 = value2;
}

void Printer::print( Kind kind, unsigned int lid, char state ){
	int index = getindex(kind, lid);
	if(buffer[index] != NULL){
		flush();
	}
	buffer[index] = new info();
	buffer[index]->state = state;
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ){
	int index = getindex(kind, lid);
	if(buffer[index] != NULL){
		flush();
	}
	buffer[index] = new info();
	buffer[index]->state = state;
	buffer[index]->value1 = value1;
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ){
	int index = getindex(kind, lid);
	if(buffer[index] != NULL){
		flush();
	}
	buffer[index] = new info();
	buffer[index]->state = state;
	buffer[index]->value1 = value1;
	buffer[index]->value2 = value2;
}