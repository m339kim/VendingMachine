#include "bottlingPlant.h"
#include <algorithm>


BottlingPlant::BottlingPlant(Printer &prt, NameServer &nameServer,
                             unsigned int numVendingMachines,
                             unsigned int maxShippedPerFlavour,
                             unsigned int maxStockPerFlavour,
                             unsigned int timeBetweenShipments)
    : printer(prt),
      nameServer(nameServer),
      (numVendingMachines),
      maxShippedPerFlavour(maxShippedPerFlavour),
      maxStockPerFlavour(maxStockPerFlavour),
      timeBetweenShipments(timeBetweenShipments) {
    truck = new Truck(printer, nameServer, *this, numVendingMachines,
                      maxStockPerFlavour);
    
    production = new unsigned int[NUM_FLAVOURS];
    std::fill(production, production + NUM_FLAVOURS, 0);
}

BottlingPlant::~BottlingPlant() {
    delete [] production;
    delete truck;
}

void BottlingPlant::main() {
    printer.print(Printer::BottlingPlant, PlantStates::Start);

    for (;;) {
        // production run 
        yield(timeBetweenShipments);
        for (unsigned int i = 0; i < NUM_FLAVOURS; i++) {
            production[i] = my_prng(maxShippedPerFlavour);
        }
        // shutdown or getShipment
        _Accept(~BottlingPlant) {
            shutdown = true;
            _Accept(getShipment);
            break;
        } or _Accept(getShipment) {}
    }
    
    printer.print(Printer::BottlingPlant, PlantStates::Finished);
}

void BottlingPlant::getShipment(unsigned int cargo[]) {
    if (shutdown) {
        // ?
        _Throw Shutdown();
    }
    for (unsigned int i = 0; i < NUM_FLAVOURS; i++) {
        cargo[i] = production[i];
    }
}
