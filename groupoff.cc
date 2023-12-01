#include "groupoff.h"

Groupoff::Groupoff(Printer& prt, unsigned int numStudents,
                   unsigned int sodaCost, unsigned int groupoffDelay)
    : printer(prt),
      numStudents(numStudents),
      sodaCost(sodaCost),
      groupoffDelay(groupoffDelay){};

WATCard::FWATCard Groupoff::giftCard() {
    WATCard::FWATCard future;
    futGiftCards.push_back(future);
    return future;
};

Groupoff::~Groupoff() {
    // should we delete from legitWATCards here, or is that the responsibility of the student?
};

void Groupoff::main() {
    printer.print(Printer::Groupoff, States::Start);

    for (unsigned int i = 0; i < numStudents; i++) {
        _Accept(giftCard){};
    }

    for (unsigned int i = 0; i < numStudents; i++) {
        _Accept(~Groupoff) {
            break;
        } _Else {
            // give a rando a giftcard
            yield(groupoffDelay);

            WATCard *w = new WATCard();
            legitWATCards.push_back(w);
            unsigned int idx = my_prng(futGiftCards.size());
            futGiftCards[idx].delivery(w);

            futGiftCards.erase(futGiftCards.begin() + idx);
            printer.print(Printer::Groupoff, States::DepositGift, sodaCost);
        }
    }

    printer.print(Printer::Groupoff, States::Finished);
};
