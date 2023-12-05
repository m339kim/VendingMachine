#include "bottlingPlant.h"

#include <algorithm>

BottlingPlant::BottlingPlant(Printer &prt, NameServer &nameServer,
                             unsigned int numVendingMachines,
                             unsigned int maxShippedPerFlavour,
                             unsigned int maxStockPerFlavour,
                             unsigned int timeBetweenShipments)
    : printer(prt),
      nameServer(nameServer),
      numVendingMachines(numVendingMachines),
      maxShippedPerFlavour(maxShippedPerFlavour),
      maxStockPerFlavour(maxStockPerFlavour),
      timeBetweenShipments(timeBetweenShipments) {
    // change here to

    production = new unsigned int[Flavours::NUM_FLAVOURS];
}

BottlingPlant::~BottlingPlant() {
    delete[] production;
    delete truck;
}

void BottlingPlant::main() {
    printer.print(Printer::BottlingPlant, PlantStates::Start);
    truck = new Truck(printer, nameServer, *this, numVendingMachines,
                      maxStockPerFlavour);
    yield(timeBetweenShipments);  // exclude first productin

    for (;;) {
        // production run
        // getShipment or shutdown
        yield(timeBetweenShipments);
        unsigned int total = 0;
        for (unsigned int i = 0; i < Flavours::NUM_FLAVOURS; i++) {
            unsigned int currStock = my_prng(maxShippedPerFlavour);
            production[i] = currStock;
            total += currStock;
        }

        
        printer.print(Printer::BottlingPlant, PlantStates::GenSoda, total);
        try {
        _Accept(getShipment){

        } or _Accept(~BottlingPlant) {
            shutdown = true;
            _Accept(getShipment);
            bench.signal();
            break;
        }

        } catch(uMutexFailure::RendezvousFailure) { break; }
    }
    printer.print(Printer::BottlingPlant, PlantStates::Finished);
}

void BottlingPlant::getShipment(unsigned int cargo[]) {
    for (unsigned int i = 0; i < NUM_FLAVOURS; i++) {
        cargo[i] = production[i];
    }


    if (shutdown) _Throw Shutdown();

    printer.print(Printer::BottlingPlant, PlantStates::PickedUp);
}
