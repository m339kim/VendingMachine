#include "printer.h"
#include <iostream>

using namespace std;

const unsigned int NUM_STATIC_KIND = 6;
const char EMPTY = '\0';

Printer::~Printer() {
    delete [] buffer;
}

Printer::Printer(unsigned int numStudents, unsigned int numVendingMachines,
                 unsigned int numCouriers)
    : numStudents(numStudents),
      numVendingMachines(numVendingMachines),
      numCouriers(numCouriers) {
    // Parent, Groupoff, WATOff, Names, Truck, Plant, Stud..., Mach..., Cour...
    unsigned int total_length = NUM_STATIC_KIND + numStudents + numVendingMachines + numCouriers;
    buffer =
        new BufferEntry[total_length];
    for (unsigned int i = 0; i < total_length; i++) {
        buffer[i].state = '\0';
    }
}

int Printer::getPosition(Kind kind, unsigned int lid) {
    switch (kind) {
        case Parent:
            return 0;
        case Groupoff:
            return 1;
        case WATCardOffice:
            return 2;
        case NameServer:
            return 3;
        case Truck:
            return 4;
        case BottlingPlant:
            return 5;
        case Student:
            return NUM_STATIC_KIND + lid;
        case Vending:
            return NUM_STATIC_KIND + numStudents + lid;
        case Courier:
            return NUM_STATIC_KIND + numStudents + numVendingMachines + lid;
        default:
            throw "impossible";
    }
}