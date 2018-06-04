#pragma once

_Monitor Bank {
	unsigned int *balance_info;									//balance information for each student
  public:
    Bank( unsigned int numStudents );
    ~Bank();
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};
