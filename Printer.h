#ifndef __PRINTER_H__
#define __PRINTER_H__
struct info{
	char state;
	int value1 = -1;
	int value2 = -1;
};

_Monitor Printer {
	unsigned int numStudents;
	unsigned int numVendingMachines;
	unsigned int numCouriers;
	unsigned int numObjects;
	info** buffer;
    void flush();
    void printinfo(info* i);
public:
    enum Kind { Parent, 
                Groupoff, 
                WATCardOffice, 
                NameServer, 
                Truck, 
                BottlingPlant, 
                Student, 
                Vending, 
                Courier 
            };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
private:
    int getindex( Kind kind, unsigned int lid );                                //use kind and id to get index in the array
};

#endif // __PRINTER_H__