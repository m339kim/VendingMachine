#include "vendingMachine.h"

#include "printer.h"

VendingMachine::VendingMachine(Printer& prt, NameServer& nameServer,
                               unsigned int id, unsigned int sodaCost)
    : printer(prt),
      nameServer(nameServer),
      id(id),
      sodaCost(sodaCost),
      restocking(false) {}

void VendingMachine::buy(Flavours flavour, WATCard& card) {
    if (stock[flavour] == 0) {
        _Throw Stock();
    } else if (my_prng(5) == 0) {
        printer.print(Printer::Vending, States::FreeSodaAd);
        _Throw Free();
    } else if (card.getBalance() < sodaCost) {
        _Throw Funds();
    }
    card.withdraw(sodaCost);
    stock[flavour] -= 1;
    printer.print(Printer::Vending, States::SodaBought, flavour,
                  stock[flavour]);
}

void VendingMachine::main() {
    printer.print(Printer::Vending, States::Start);
    nameServer.VMregister(this);

    for (;;) {
        _Accept(~VendingMachine) { break; }
        or
            _When(!restocking) _Accept(buy){

            } or
            _When(!restocking) _Accept(inventory){

            } or
            _When(restocking) _Accept(restocked) {}
    }

    printer.print(Printer::Vending, States::Finished);
}

unsigned int* VendingMachine::inventory() {
    restocking = true;
    printer.print(Printer::Vending, States::StartTruckReload);
    return stock;
}
void VendingMachine::restocked() {
    restocking = false;
    printer.print(Printer::Vending, States::DoneTruckReload);
};
unsigned int VendingMachine::cost() const { return sodaCost; }
unsigned int VendingMachine::getId() const { return id; }