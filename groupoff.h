#pragma once

#include "WATCard.h"
#include <uPRNG.h>
#include <vector>
using namespace std;

_Monitor Printer;

extern PRNG my_prng;

_Task Groupoff {
    Printer &printer;

	enum States : char {
		Start = 'S',
		DepositGift = 'D', // g // amount of gift g
		Finished = 'F'
	};
	
	unsigned int numStudents;
    unsigned int sodaCost; 
    unsigned int groupoffDelay;

	vector< WATCard::FWATCard > futGiftCards;
	vector< WATCard* > legitWATCards;

	void main();
	public:
        Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
        WATCard::FWATCard giftCard();
        ~Groupoff();
};