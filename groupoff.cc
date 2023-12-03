#include "groupoff.h"

Groupoff::Groupoff(Printer& prt, unsigned int numStudents,
                   unsigned int sodaCost, unsigned int groupoffDelay)
    : printer(prt),
      numStudents(numStudents),
      sodaCost(sodaCost),
      groupoffDelay(groupoffDelay) {};

WATCard::FWATCard Groupoff::giftCard() {
    WATCard::FWATCard futureGiftCard;
    futGiftCards.push_back(futureGiftCard);
    return futureGiftCard;
}

Groupoff::~Groupoff() {
    // should we delete from legitWATCards here, or is that the responsibility of the student?
    // -> we delete them here!
    for (int i = 0; i < legitWATCards.size(); i++) {
        delete legitWATCards.at(i); // dealloc
    }
}

void Groupoff::main() {
    printer.print(Printer::Groupoff, Groupoff::States::Start);

    for (unsigned int i = 0; i < numStudents; i++) {
        _Accept(giftCard){};
    }

    for (unsigned int i = 0; i < numStudents; i++) {
        _Accept(~Groupoff) {
            break;
        } _Else {
            // give a rando a giftcard
            yield(groupoffDelay);

            WATCard * w = new WATCard(); 
            w->deposit(sodaCost); // implement
            legitWATCards.push_back(w);
            unsigned int student_idx = my_prng(futGiftCards.size());
            // print before or after delivery?
            // printer.print(Printer::Kind::Groupoff, Groupoff::States::DepositGift, sodaCost);
            futGiftCards[student_idx].delivery(w);

            futGiftCards.erase(futGiftCards.begin() + student_idx);
            printer.print(Printer::Kind::Groupoff, States::DepositGift, sodaCost);
        }
    }
    printer.print(Printer::Groupoff, Groupoff::States::Finished);
}
