#pragma once
#include <uPRNG.h>

_Task NameServer;
_Monitor Printer;

extern PRNG my_prng;

_Task VendingMachine {
	Printer &printer;
	NameServer &nameServer;

	unsigned int id;
	unsigned int sodaCost;
	unsigned int stock[4] = {0,0,0,0};
	bool restocking;

	uCondition bench;

	enum States : char {
		Start = 'S', // c // cost c per bottle
		StartTruckReload = 'r',
		DoneTruckReload = 'R',
		FreeSodaAd = 'A',
		SodaBought = 'B', // f,r // soda flavour f purchased, amount remaining r of this flavour
		Finished = 'F'
	};
	enum Flavours { BLACK_CHERRY, CREAM_SODA, ROOT_BEER, JAZZ_LIME, NUM_FLAVOURS };	// flavours of soda (YOU DEFINE)

	void main();

	public:
		_Event Free {};						// free, advertisement
		_Event Funds {};					// insufficient funds
		_Event Stock {};					// flavour out of stock
		VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );
		void buy( Flavours flavour, WATCard & card );
		unsigned int * inventory() __attribute__(( warn_unused_result ));
		void restocked();
		_Nomutex unsigned int cost() const;
		_Nomutex unsigned int getId() const;
	private:
		Flavours curFlavour = 0; // BLACK_CHERRY
		WATCard * watcard = nullptr;
};