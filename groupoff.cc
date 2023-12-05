#include "groupoff.h"

#include "printer.h"

Groupoff::Groupoff(Printer& prt, unsigned int numStudents,
                   unsigned int sodaCost, unsigned int groupoffDelay)
    : printer(prt),
      numStudents(numStudents),
      sodaCost(sodaCost),
      groupoffDelay(groupoffDelay){};

WATCard::FWATCard Groupoff::giftCard() {
    bench.wait();

    return futGiftCards.back();
}

Groupoff::~Groupoff() {
    for (int i = 0; i < legitWATCards.size(); i++) {
        delete legitWATCards.at(i);  // dealloc
    }
}

void Groupoff::main() {
    printer.print(Printer::Groupoff, Groupoff::States::Start);

    for (unsigned int i = 0; i < numStudents; i++) {
        _Accept(giftCard) {
            WATCard::FWATCard futureGiftCard;
            futGiftCards.push_back(futureGiftCard);
            bench.signal();
        };
    }

    for (unsigned int i = 0; i < numStudents; i++) {
        _Accept(~Groupoff) { break; }
        _Else {
            // give a rando a giftcard
            yield(groupoffDelay);

            WATCard* w = new WATCard();
            w->deposit(sodaCost);
            legitWATCards.push_back(w);
            unsigned int student_idx = my_prng(futGiftCards.size());

            futGiftCards[student_idx].delivery(w);

            // print after delivery
            printer.print(Printer::Kind::Groupoff,
                          Groupoff::States::DepositGift, sodaCost);
        }
    }
    printer.print(Printer::Groupoff, Groupoff::States::Finished);
}
