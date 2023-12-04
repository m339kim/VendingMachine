#pragma once
#include "vendingMachine.h"
#include "printer.h"

_Task NameServer {
    private:
        Printer & printer;
        uCondition bench;

        enum States : char {
            Start = 'S',
            RegisterVM = 'R', // v // vending machine v registering
            NewVM = 'N', // s,v // student s requesting vending machine, new vending machine v
            Finished = 'F'
        };

        unsigned int numVendingMachines;
        unsigned int numStudents;
        unsigned int studentId;

        VendingMachine *newMachine;
        VendingMachine **machines;
        unsigned int machineIndex = 0;

        void main();
	public:
        NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
        void VMregister( VendingMachine * vendingmachine );
        VendingMachine * getMachine( unsigned int id ) __attribute__(( warn_unused_result ));
        VendingMachine ** getMachineList() __attribute__(( warn_unused_result ));
};
