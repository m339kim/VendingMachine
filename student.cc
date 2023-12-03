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
    unsigned int favouriteFlavour = (VendingMachine::Flavours) my_prng(BottlingPlant::Flavours::NUM_FLAVOURS);
    printer.print(Printer::Kind::Student, id, States::Start, favouriteFlavour, bottlesToPurchase); // add bottlesToPurchanse

    WATCard::FWATCard watcard = cardOffice.create(id, 5); // create watcard
    WATCard::FWATCard giftcard = groupoff.giftCard();   // create giftcard

    VendingMachine * currMachine = nameServer.getMachine(id);
    printer.print(Printer::Kind::Student, 
                    id, 
                    Student::States::SelectingVM,
                    currMachine->getId()
                  );

    char cardType = '\0';
    WATCard * cardToUse;

    for (unsigned int i = 0; i < bottlesToPurchase; i++) {
        _Select(giftcard || watcard) { // either card available
            try {
                // use giftcard first if available
                if (giftcard.available()) {
                    cardToUse = giftcard();
                    cardType = Student::States::GiftCardSoda;
                }  else if (watcard.available()) {
                    cardToUse = watcard();
                    cardType = Student::States::BoughtSoda;
                }

                yield(my_prng(1, 10));

                // make soda purchase
                currMachine->buy(favouriteFlavour, *cardToUse);
                printer.print(Printer::Student, id, cardType, favouriteFlavour,
                              cardToUse->getBalance());
                if(giftcard.available()) giftcard.reset();
            } catch (WATCardOffice::LostWATCard &e) { // courier lost student's WATCard during transfer
                printer.print(Printer::Kind::Student, id, Student::States::WATCardLost);
                watcard.reset();
                watcard = cardOffice.create(id, 5); // initial $5 balance
            } catch (VendingMachine::Free &e) {
                // 50% chance to watch ad
                // XXX: wtf are we supposed to print if they got the free soda
                // but didn't watch the ad? I guess just X?
                // this is confusing..
                if (my_prng(2) == 0) { // watch ad
                    // logic here is so messed pls send help!!!!
                    if (giftcard.available()) {
                        // Gotta handle edge case where:
                        //  student who only buys one soda uses the gift card.
                        if (cardType == States::BoughtSoda) {
                            cardType = Student::States::FreeSodaAdGC;
                        } else {
                            cardType = States::BoughtSoda;
                        }
                    } else if (watcard.available()){
                        if (cardType == States::BoughtSode) {
                            cardType = Student::States::FreeSodaAdWC;
                        } else {
                            cardType = States::BoughtSoda;
                        }
                    }
                    printer.print(Printer::Kind::Student, id, cardType,
                        favouriteFlavour, cardToUse->getBalance());
                    yield(4); // watch ad
                } else {    // don't watch ad
                    // ??? help
                }
            } catch (VendingMachine::Funds &e) {
                // gift card is guaranteed to have enough money I think
                watcard = cardOffice.transfer(id, 5 + currMachine.cost(), cardToUse);
            } catch (VendingMachine::Stock &e) {
                currMachine = nameServer.getMachine(id);
                printer.print(Printer::Student, id, Student::States::SelectingVM,
                              currMachine->getId());
            }
        }
    }
    printer.print(Printer::Student, id, Student::States::Finished);
}    
