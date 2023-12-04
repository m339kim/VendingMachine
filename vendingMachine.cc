#include "vendingMachine.h"
#include "printer.h"
#include "WATCard.h"
#include "nameServer.h"

VendingMachine::VendingMachine(Printer& prt, NameServer& nameServer,
                               unsigned int id, unsigned int sodaCost)
    : printer(prt),
      nameServer(nameServer),
      id(id),
      sodaCost(sodaCost),
      restocking(false) { }

void VendingMachine::buy(Flavours flavour, WATCard& card) {
    watcard = &card;
    curFlavour = flavour;
    bench.wait();
    if (stock[flavour] == 0) {
        _Throw Stock();
    } else if (card.getBalance() < sodaCost) {
        _Throw Funds();
    } else {
        stock[curFlavour] -= 1;
        if (my_prng(5-1) == 0) {
            printer.print(Printer::Vending, getId(), States::FreeSodaAd);
            _Throw Free();
        } else {
            watcard->withdraw(sodaCost);
            printer.print(Printer::Vending, States::SodaBought, flavour,
                        stock[flavour]);
        }
    }
    bench.signalBlock();
}

void VendingMachine::main() {
    printer.print(Printer::Vending, States::Start, sodaCost);
    nameServer.VMregister(this);

    for (;;) {
        try {
            _Accept(~VendingMachine) {
                printer.print(Printer::Vending, States::Finished);
                break; 
            }
            or
                _When(!restocking) _Accept(buy){
                    bench.signal();
                } or
                _When(!restocking) _Accept(inventory){

                } or
                _When(restocking) _Accept(restocked) {}
        } catch(uMutexFailure::RendezvousFailure &) {}
    }
}

unsigned int * VendingMachine::inventory() {
    restocking = true;
    printer.print(Printer::Vending, States::StartTruckReload);
    return stock;
}
void VendingMachine::restocked() {
    restocking = false;
    printer.print(Printer::Vending, States::DoneTruckReload);
};
_Nomutex unsigned int VendingMachine::cost() const { 
    return sodaCost; 
}
_Nomutex unsigned int VendingMachine::getId() const { 
    return id; 
}