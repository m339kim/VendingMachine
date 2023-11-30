#include "printer.h"

#include <iostream>

using namespace std;

const unsigned int NUM_STATIC_KIND = 6;
const char EMPTY = '\0';

Printer::~Printer() { delete[] buffer; }

Printer::Printer(unsigned int numStudents, unsigned int numVendingMachines,
                 unsigned int numCouriers)
    : numStudents(numStudents),
      numVendingMachines(numVendingMachines),
      numCouriers(numCouriers),
      totalLength(NUM_STATIC_KIND + numStudents + numVendingMachines +
                  numCouriers) {
    // Parent, Groupoff, WATOff, Names, Truck, Plant, Stud..., Mach..., Cour...
    buffer = new BufferEntry[totalLength];
    for (unsigned int i = 0; i < totalLength; i++) {
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

void Printer::print(Kind kind, char state) {
    innerPrint(kind, 0, state, 0, 0, 0);
}
void Printer::print(Kind kind, char state, unsigned int value1) {
    innerPrint(kind, 0, state, value1, 0, 1);
}
void Printer::print(Kind kind, char state, unsigned int value1, unsigned int value2){
    innerPrint(kind, 0, state, value1, value2, 2);
};
void Printer::print(Kind kind, unsigned int lid, char state){
    innerPrint(kind, lid, state, 0, 0, 0);
}
void Printer::print(Kind kind, unsigned int lid, char state, unsigned int value1){
    innerPrint(kind, lid, state, value1, 0, 1);
}
void Printer::print(Kind kind, unsigned int lid, char state, unsigned int value1,
           unsigned int value2){
    innerPrint(kind, lid, state, value1, value2, 2);
}

void Printer::innerPrint(Kind kind, unsigned int lid, char state,
                         unsigned int value1, unsigned int value2,
                         unsigned int numValues) {
    int pos = getPosition(kind, lid);
    if (buffer[pos].state != '\0') {
        // buffer isn't empty - need to flush
        flush();
    }
    buffer[pos].state = state;
    buffer[pos].value1 = value1;
    buffer[pos].value2 = value2;
    buffer[pos].numValues = numValues;
}

void Printer::flush() { 
    for (unsigned int i = 0; i < totalLength; i++) {
        // print every buffer
        if (buffer[i].state != '\0') {
            cout << buffer[i].state;
        }
        if (buffer[i].numValues >= 1) {
            cout << buffer[i].value1;
        }
        if (buffer[i].numValues == 2) {
            cout << ",";
            cout << buffer[i].value2;
        }
        cout << "\t";
        buffer[i].state = '\0';
    }
    cout << endl;
 }