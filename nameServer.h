#pragma once
#include "vendingMachine.h"

_Monitor Printer;

_Task NameServer {
    private:
        Printer &printer;

        enum States : char {
            Start = 'S',
            RegisterVM = 'R', // v // vending machine v registering
            NewVM = 'N', // s,v // student s requesting vending machine, new vending machine v
            Finished = 'F'
        };

        unsigned int numVendingMachines;
        unsigned int numStudents;
        unsigned int *studentIdxs;

        VendingMachine *newMachine;
        VendingMachine **machines;
        unsigned int machineIndex = 0;

	public:
        NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
        void main();
    ~NameServer();
        void VMregister( VendingMachine * vendingmachine );
        VendingMachine * getMachine( unsigned int id ) __attribute__(( warn_unused_result ));
        VendingMachine ** getMachineList() __attribute__(( warn_unused_result ));
};
