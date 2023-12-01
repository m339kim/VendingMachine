#pragma once
#include "nameServer.h"
#include "printer.h"
#include "truck.h"
#include <uPRNG.h>

PRNG my_prng;

_Task BottlingPlant {
  Printer &printer;
	NameServer &nameServer;
  Truck * truck;
  uCondition bench;

	enum PlantStates : char {
		Start = 'S',
		GenSoda = 'G', // b // bottles b generated in production run
		PickedUp = 'P',
		Finished = 'F'
	};

	unsigned int numVendingMachines;
  unsigned int maxShippedPerFlavour;
  unsigned int maxStockPerFlavour;
  unsigned int timeBetweenShipments;

  unsigned int *production;

	bool shutdown = false;	// plant shutdown
	
	void main();

  public:
    enum Flavours { BLACK_CHERRY, CREAM_SODA, ROOT_BEER, JAZZ_LIME, NUM_FLAVOURS };	// flavours of soda (YOU DEFINE)
    _Event Shutdown {};					// shutdown plant exception
    BottlingPlant ( 
      Printer &prt, 
      NameServer &nameServer, 
      unsigned int numVendingMachines,
      unsigned int maxShippedPerFlavour, 
      unsigned int maxStockPerFlavour,
      unsigned int timeBetweenShipments 
    );
    ~BottlingPlant();
    void getShipment( unsigned int cargo[] );
};