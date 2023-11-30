#pragma once

#include <uPRNG.h>

_Monitor Printer;
_Monitor Bank;

PRNG my_prng;


_Task Parent {
    private:
        Printer & printer;
        Bank & bank;

        enum States : char {
            Start = 'S',
            DepositGift = 'D', // s,g // student s receiving gift, amount of gift g
            Finished = 'F'
        };
        
        unsigned int numStudents;
        unsigned int parentalDelay;
        
        void main();
  public:
	Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay );
    ~Parent();
};
