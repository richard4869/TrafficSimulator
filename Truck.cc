#include "Truck.h"
#include "MPRNG.h"
#include "BottlingPlant.h"
extern MPRNG mprng;
Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour ):
	printer(prt),
	nameServer(nameServer),
	plant(plant),
	numVendingMachines(numVendingMachines),
	maxStockPerFlavour(maxStockPerFlavour){
		for (int i = 0; i < VendingMachine::Flavours::Num; i++){
			cargo[i] = 0;
		}
	}


void Truck::main(){
	printer.print(Printer::Kind::Truck, 'S');
	
	try {
		VendingMachine **machine_list = nameServer.getMachineList();
		int current_machine = 0;

		for (;;) {
			yield(mprng(1, 10));									// Tom Hortons
			plant.getShipment(cargo);
			int cargo_count = 0;

			for (int i = 0; i < VendingMachine::Flavours::Num; i++){
				cargo_count += cargo[i];
			}

			printer.print(Printer::Kind::Truck, 'P', cargo_count);

			cycle:for(;;){
				unsigned int *machine_inventory = machine_list[current_machine]->inventory();
				printer.print(Printer::Kind::Truck, 'd', current_machine, cargo_count);

				int still_missing = VendingMachine::Flavours::Num * maxStockPerFlavour;

				for(int i = 0; i < VendingMachine::Flavours::Num; i++){
					unsigned int restock = maxStockPerFlavour - machine_inventory[i];
					if (restock > cargo[i]) restock = cargo[i];
					cargo[i] -= restock;												//move it from truck
					machine_inventory[i] += restock;									//add it to vm
					still_missing -= machine_inventory[i];
					cargo_count -= restock;
				}
				if(still_missing > 0) printer.print(Printer::Kind::Truck, 'U', current_machine, still_missing);		//unsuccessful
				printer.print(Printer::Kind::Truck, 'D', current_machine, cargo_count);								//end of delevery
				machine_list[current_machine]->restocked();																//restocked

				current_machine = (current_machine + 1) % numVendingMachines;
				if (cargo_count == 0 || current_machine == 0) break cycle;
			}
		}
	}catch(BottlingPlant::Shutdown e){
		printer.print(Printer::Kind::Truck, 'F');
	}
}