#include "vendingMachine.h"

#include "WATCard.h"
#include "nameServer.h"
#include "printer.h"

VendingMachine::VendingMachine(Printer& prt, NameServer& nameServer,
                               unsigned int id, unsigned int sodaCost)
    : printer(prt),
      nameServer(nameServer),
      id(id),
      sodaCost(sodaCost),
      restocking(false) {}

void VendingMachine::buy(Flavours flavour, WATCard& card) {
    watcard = &card;
    curFlavour = flavour;
    currTask = &resumer();
    bench.wait();
}

void VendingMachine::main() {
    printer.print(Printer::Vending, id, States::Start, sodaCost);
    nameServer.VMregister(this);

    for (;;) {
        try {
            _Accept(~VendingMachine) {
                printer.print(Printer::Vending, id, States::Finished);
                break;
            }
            or _When(!restocking) _Accept(buy) {
                if (stock[curFlavour] == 0) {
                    _Resume Stock() _At* currTask;
                } else if (card.getBalance() < sodaCost) {
                    _Throw Funds() _At* currTask;
                } else {
                    stock[curFlavour] -= 1;
                    if (my_prng(5 - 1) == 0) {
                        printer.print(Printer::Vending, id, States::FreeSodaAd);
                        _Throw Free() _At* currTask;
                    } else {
                        watcard->withdraw(sodaCost);
                        printer.print(Printer::Vending, id, States::SodaBought,
                                      curFlavour, stock[curFlavour]);
                    }
                }
                bench.signalBlock();
            }
            or
                _When(!restocking) _Accept(inventory){

                } or
                _When(restocking) _Accept(restocked) {}
        } catch (uMutexFailure::RendezvousFailure&) {
        }
    }
}

unsigned int* VendingMachine::inventory() {
    restocking = true;
    printer.print(Printer::Vending, id, States::StartTruckReload);
    return stock;
}
void VendingMachine::restocked() {
    restocking = false;
    printer.print(Printer::Vending, id, States::DoneTruckReload);
};
_Nomutex unsigned int VendingMachine::cost() const { return sodaCost; }
_Nomutex unsigned int VendingMachine::getId() const { return id; }