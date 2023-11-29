#pragma once

// mj TODO - not done !!!!

_Task Truck {
	enum States : char {
		Start = 'S',
		PickedUp = 'P', // a // total amount a of all sodas in the shipment
		BeginDelivery = 'd', // v,r // vending machine v, total amount remaining r in the shipment
		FailedFillingVM = 'U', // v,b // vending machine v, total number of bottles b not replenished
		DoneDelivery = 'D', // v,r // vending machine v, total amount remaining r in the shipment
		WaitForRepair = 'W',
		Finished = 'F'
	};

	void main();
  public:
	Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
		   unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
    ~Truck();
};