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
    production = new unsigned int[Flavours::NUM_FLAVOURS];
}

BottlingPlant::~BottlingPlant() {
    delete[] production;
    delete truck;
}

void BottlingPlant::main() {
    printer.print(Printer::BottlingPlant, PlantStates::Start);

    for (;;) {
        // production run
        yield(timeBetweenShipments);
        unsigned int total = 0;
        for (unsigned int i = 0; i < Flavours::NUM_FLAVOURS; i++) {
            unsigned int currStock = my_prng(maxShippedPerFlavour);
            production[i] = currStock;
            total += currStock;
        }
        printer.print(Printer::BottlingPlant, PlantStates::GenSoda, total);
        // shutdown or getShipment
        _Accept(~BottlingPlant) {
            shutdown = true;
            _Accept(getShipment);
            break;
        }
        or _Accept(getShipment) {}
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
    printer.print(Printer::BottlingPlant, PlantStates::PickedUp);
}
