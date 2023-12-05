#include "student.h"

Student::Student(Printer& prt, NameServer& nameServer,
                 WATCardOffice& cardOffice, Groupoff& groupoff, unsigned int id,
                 unsigned int maxPurchases)
    : printer(prt),
      nameServer(nameServer),
      cardOffice(cardOffice),
      groupoff(groupoff),
      id(id),
      maxPurchases(maxPurchases) {
    return;
}

Student::~Student() {}

void Student::main() {
    unsigned int bottlesToPurchase = my_prng(1, maxPurchases);
    unsigned int favouriteFlavour = (VendingMachine::Flavours)my_prng(
        BottlingPlant::Flavours::NUM_FLAVOURS);
    printer.print(Printer::Kind::Student, id, States::Start, favouriteFlavour,
                  bottlesToPurchase);  // add bottlesToPurchanse

    WATCard::FWATCard watcard = cardOffice.create(id, 5);  // create watcard
    WATCard::FWATCard giftcard = groupoff.giftCard();      // create giftcard

    VendingMachine* currMachine = nameServer.getMachine(id);
    printer.print(Printer::Kind::Student, id, Student::States::SelectingVM,
                  currMachine->getId());

    char cardType = '\0';
    WATCard* cardToUse;
    for (unsigned int i = 0; i < bottlesToPurchase; i++) {
        yield(my_prng(1, 10));
        for (;;) {
            _Select(giftcard || watcard) {
                if (giftcard.available()) {
                    cardToUse = giftcard();
                    giftcard.reset();
                    cardType = States::GiftCardSoda;
                } else {
                    try {
                        cardToUse = watcard();
                        cardType = States::BoughtSoda;
                    } catch (WATCardOffice::Lost& lost) {
                        printer.print(Printer::Kind::Student, id, Student::States::WATCardLost);
                        watcard = cardOffice.create(id, 5);
                        continue;
                    }
                }
            }
            try {
                currMachine->buy((VendingMachine::Flavours)favouriteFlavour,
                                 *cardToUse);
                printer.print(Printer::Student, id, cardType, favouriteFlavour,
                              cardToUse->getBalance());
                break;
            } catch (VendingMachine::Free& free) {
                if (cardType == States::BoughtSoda) {
                    cardType = States::FreeSodaAdWC;
                } else {
                    cardType = States::FreeSodaAdGC;
                }
                printer.print(Printer::Student, id, cardType, favouriteFlavour,
                              cardToUse->getBalance());
                if (my_prng(2) == 0) {
                    yield(4);
                } else {
                    cardType = States::SkippedAd;
                    printer.print(Printer::Student, id, cardType);
                }
                break;
            } catch (VendingMachine::Funds& funds) {
                // gift card is guaranteed to have enough money I think
                watcard =
                    cardOffice.transfer(id, 5 + currMachine->cost(), cardToUse);
                continue;
            } catch (VendingMachine::Stock& stock) {
                currMachine = nameServer.getMachine(id);
                printer.print(Printer::Student, id, States::SelectingVM,
                              currMachine->getId());
                continue;
            }
        }
    }

    try {
        delete watcard();
    } catch (...) {
    }

    printer.print(Printer::Student, id, States::Finished);
}
