#include "Bank.h"

Bank::Bank( unsigned int numStudents ){
	balance_info = new unsigned int [numStudents];
	for (unsigned int i = 0; i < numStudents; i++) {
		balance_info[i] = 0;
	}
}

Bank::~Bank(){
	delete[] balance_info;
}

void Bank::deposit( unsigned int id, unsigned int amount ){
	balance_info[id] += amount;
}

void Bank::withdraw( unsigned int id, unsigned int amount ){
	for(;;){												//until get enough money
		if(balance_info[id] < amount){						//not enough monney
			_Accept(Bank::deposit);							//wait for deposit
		}else{
			break;
		}
	}
	balance_info[id] -= amount;
}
