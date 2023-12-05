#include "truck.h"
#include <uPRNG.h>
#include "printer.h"
#include "bottlingPlant.h"
#include "nameServer.h"


Truck::Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour ):
    printer(prt), 
    nameServer(nameServer),
    plant(plant), 
    numVendingMachines(numVendingMachines), 
    maxStockPerFlavour(maxStockPerFlavour)
    {
        cargo = new unsigned int[4];
        for (unsigned int i = 0; i < 4; i++) {
            cargo[i] = 0;
        } 
    }

Truck::~Truck(){
    delete [] cargo;
    printer.print(Printer::Kind::Truck, Truck::States::Finished);
}

void Truck::main() {
    printer.print(Printer::Kind::Truck, Truck::States::Start);
    machines = nameServer.getMachineList();

    for (;;) {
        yield(my_prng(1, 10)); // coffee break        
        
        // get new shipment
        try {
            plant.getShipment(cargo);
            printer.print(Printer::Kind::Truck, Truck::States::PickedUp, shipmentCount());
        } catch (BottlingPlant::Shutdown &e) {
            break;
        }
        
        unsigned int curr = machineIndex;
        for (;;) {
            if (!(cargo[0] || cargo[1] || cargo[2] || cargo[3])) { // no cargo
                break;
            }
            VendingMachine *vm = machines[curr];
            fillStock(vm);
            curr += 1;
            curr = curr % numVendingMachines;
            if (curr == machineIndex) break; // delivered to all vms
        }
        machineIndex = curr; // reset
    } _Accept(~Truck);
}

void Truck::fillStock(VendingMachine * machine) {
    printer.print(Printer::Kind::Truck, Truck::States::BeginDelivery, machine->getId(), shipmentCount());
    int noReplen = 0;
    unsigned int * stock = machine->inventory();
    
    for (int i = 0; i < 4; i++) {
        unsigned int fill = std::min(maxStockPerFlavour - stock[i], cargo[i]);
        noReplen += maxStockPerFlavour - stock[i] - fill;
        stock[i] += fill;
        cargo[i] -= fill;
    }
    // failed filling vend
    if (noReplen > 0) printer.print(Printer::Kind::Truck, Truck::States::FailedFillingVM, machine->getId(), noReplen);
    printer.print(Printer::Kind::Truck, Truck::States::DoneDelivery, machine->getId(), shipmentCount());
    machine->restocked();

    // have flat tire by 1% chance
    if (my_prng(100) == 0) {
        printer.print(Printer::Kind::Truck, Truck::States::WaitForRepair);
        yield(10);
    } 
}

// counts total shipments in cargo[]
unsigned int Truck::shipmentCount() {
    int total = 0;
    for (int i = 0; i < 4; i++) {
        total += cargo[i];
    }
    return total;
}