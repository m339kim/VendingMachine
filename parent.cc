#include "parent.h"

#include "bank.h"
#include "printer.h"

Parent::Parent(Printer& prt, Bank& bank, unsigned int numStudents,
               unsigned int parentalDelay)
    : printer(prt),
      bank(bank),
      numStudents(numStudents),
      parentalDelay(parentalDelay) {}

void Parent::main() {
    printer.print(Printer::Parent, Start);
    for (;;) {
        _Accept(~Parent) { break; }
        _Else {
            yield(parentalDelay);
            int amount = my_prng(1, 3);
            int id = my_prng(numStudents - 1);
            bank.deposit(id, amount);
            printer.print(Printer::Parent, id, Parent::States::DepositGift, 
                          amount);
        }
    }
    printer.print(Printer::Parent, Finished);
}

Parent::~Parent() {}