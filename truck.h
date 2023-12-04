#pragma once

_Task VendingMachine;
_Monitor Printer;

extern PRNG my_prng;

_Task Truck {
	NameServer & nameServer;
	BottlingPlant & plant;
	VendingMachine **machines;
	Printer & printer;

	enum States : char {
		Start = 'S',
		PickedUp = 'P', // a // total amount a of all sodas in the shipment
		BeginDelivery = 'd', // v,r // vending machine v, total amount remaining r in the shipment
		FailedFillingVM = 'U', // v,b // vending machine v, total number of bottles b not replenished
		DoneDelivery = 'D', // v,r // vending machine v, total amount remaining r in the shipment
		WaitForRepair = 'W',
		Finished = 'F'
	};

	unsigned int numVendingMachines;
	unsigned int maxStockPerFlavour;

	unsigned int * cargo = 0;
	unsigned int * machineIndex = 0;


	void main();
	unsigned int shipmentCount();
  public:
	Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
		   unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
    ~Truck();
};