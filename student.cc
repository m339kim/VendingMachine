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
    printer.print(Printer::Student, id, States::Start);
    unsigned int bottlesToPurchase = my_prng(1, maxPurchases);
    unsigned int favouriteFlavour =
        my_prng(BottlingPlant::Flavours::NUM_FLAVOURS);
    WATCard::FWATCard watcard = cardOffice.create(id, 5);
    WATCard::FWATCard giftcard = groupoff.giftCard();
    VendingMachine* currMachine = nameServer.getMachine(id);
    printer.print(Printer::Student, id, States::SelectingVM,
                  currMachine.getId());
    char cardType = '\0';
    WATCard* cardToUse;

    for (unsigned int i = 0; i < bottlesToPurchase; i++) {
        yield(my_prng(1, 10));
        for (;;) {
            _Select(giftcard) {
                WATCard* cardToUse = giftcard();
                giftcard.reset();
                cardType = States::GiftCardSoda;
            }
            or _Select(watcard) {
                try {
                    cardToUse = watcard();
                    cardType = States::BoughtSoda;
                } catch (WATCardOffice::Lost) {
                    watcard = cardOffce.create(id, 5);
                    continue;
                }
            }
            try {
                currMachine.buy(favouriteFlavour, card);
                printer.print(Printer::Student, id, cardType, favouriteFlavour,
                              card->getBalance());
            } catch (VendingMachine::Free free) {
                // 50% chance to watch ad
                // XXX: wtf are we supposed to print if they got the free soda but didn't watch the ad? I guess just X?
                if (my_prng(2) == 0) {
                    yield(4);
                    if (cardType == States::BoughtSoda) {
                        cardType = States::FreeSodaAdGC;
                    } else {
                        cardType = States::FreeSodaAdGC;
                    }
                } else {
                    cardType = States::SkippedAd;
                }
                printer.print(Printer::Student, id, cardType, favouriteFlavour,
                              card->getBalance());
            } catch (VendingMachine::Funds funds) {
                // gift card is guaranteed to have enough money I think
                watcard =
                    cardOffice.transfer(id, 5 + currMachine.cost(), cardToUse);
                continue;
            } catch (VendingMachine::Stock) {
                currMachine = nameServer.getMachine(id);
                printer.print(Printer::Student, id, States::SelectingVM,
                              currMachine.getId());
                continue;
            }
        }
    }
    printer.print(Printer::Student, id, States::Finished);
}
