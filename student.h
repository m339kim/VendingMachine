#pragma once
#include "vendingMachine.h"
#include "WATCardOffice.h"
#include "groupoff.h"
#include "nameServer.h"
#include "bottlingPlant.h"

#include <uPRNG.h>

_Monitor Printer;
_Task WATCardOffice;
extern PRNG my_prng;

_Task Student {
	Printer &printer;
    NameServer &nameServer;
    WATCardOffice &cardOffice;
    Groupoff &groupoff;

    enum States : char {
        Start = 'S', // f,b // favourite soda f, number of bottles b to purchase
        SelectingVM = 'V', // v // vending machine v selected
        GiftCardSoda = 'G', // f,b // soda flavour f purchased, giftcard balance b
        BoughtSoda = 'B', // f,b // soda flavour f purchased, WATCard balance b
        FreeSodaAdGC = 'a', // f,b // soda flavour f purchased, giftcard balance b
        FreeSodaAdWC = 'A', // f,b // soda flavour f purchased, WATCard balance b
        SkippedAd = 'X',
        WATCardLost = 'L',
        Finished = 'F'
    };

	unsigned int id;
    unsigned int maxPurchases;

	void main();
	public:
	    Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
		    unsigned int id, unsigned int maxPurchases );
	    ~Student();
};
