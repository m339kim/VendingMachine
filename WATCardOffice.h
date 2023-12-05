#pragma once

#include "WATCard.h"
#include <vector>
#include <uPRNG.h>

_Monitor Printer;
_Monitor Bank;
extern PRNG my_prng;

using namespace std;

_Task WATCardOffice {
	struct Job {					 // marshalled arguments and return future
        // Args args:			// call arguments (YOU DEFINE "Args")
        unsigned int sid;     // student id
		unsigned int amount;
		WATCard * watcard;

		WATCard::FWATCard result;			// return future
		Job( unsigned int sid, unsigned int amount, WATCard * card ) : sid(sid), amount(amount), watcard(card) {}
	};
	_Task Courier { // communicate with bank
    
        Printer &printer;
		Bank &bank;
		WATCardOffice &office;
        unsigned int cid;

		enum States : char {
			Start = 'S',
			StartTransfer = 't', // s,a // student s requesting transfer, amount a of transfer
			WATCardLost = 'L', // s // student s requesting transfer
			DoneTransfer = 'T', // s,a // student s requesting transfer, amount a of transfer
			Finished = 'F'
		};

		void main();

		public:
            Courier(Printer & printer, Bank & bank, WATCardOffice & office, unsigned int cid);
            ~Courier();
	};

	enum States : char {
		Start = 'S',
		RequestComplete = 'W',
		CreateComplete = 'C', // s,a // student s, transfer amount a
		TransferComplete = 'T', // s,a // student s, transfer amount a
		Finished = 'F'
	};

	Printer &printer;
	Bank &bank;
	
	unsigned int numCouriers;
	Courier ** couriers;
	vector<Job *> jobs;
	uCondition bench;
	unsigned int currAmount;
	unsigned int currSid;
	WATCard *currWATCard;
	void main();

	public:
        _Event Lost {};							// lost WATCard
        WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
        ~WATCardOffice();
        WATCard::FWATCard create( unsigned int sid, unsigned int amount ) __attribute__(( warn_unused_result ));
        WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card ) __attribute__(( warn_unused_result ));
        Job * requestWork() __attribute__(( warn_unused_result ));
};
