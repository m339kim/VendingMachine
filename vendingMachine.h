#pragma once
#include <uPRNG.h>

_Task NameServer;
_Monitor Printer;
class WATCard;

extern PRNG my_prng;

_Task VendingMachine {
    Printer & printer;
    NameServer & nameServer;

    unsigned int id;
    unsigned int sodaCost;
    unsigned int stock[4] = {0, 0, 0, 0};
    bool restocking;

    uCondition bench;
    uBaseCoroutine* currTask;

    enum States : char {
        Start = 'S',  // c // cost c per bottle
        StartTruckReload = 'r',
        DoneTruckReload = 'R',
        FreeSodaAd = 'A',
        SodaBought = 'B',  // f,r // soda flavour f purchased, amount remaining
                           // r of this flavour
        Finished = 'F'
    };

    void main();

   public:
    enum Flavours {
        BLACK_CHERRY,
        CREAM_SODA,
        ROOT_BEER,
        JAZZ_LIME,
        NUM_FLAVOURS
    };               // flavours of soda (YOU DEFINE)
    _Event Free{};   // free, advertisement
    _Event Funds{};  // insufficient funds
    _Event Stock{};  // flavour out of stock
    VendingMachine(Printer & prt, NameServer & nameServer, unsigned int id,
                   unsigned int sodaCost);
    void buy(Flavours flavour, WATCard & card);
    unsigned int* inventory() __attribute__((warn_unused_result));
    void restocked();
    _Nomutex unsigned int cost() const;
    _Nomutex unsigned int getId() const;

   private:
    Flavours curFlavour = Flavours::BLACK_CHERRY;  // BLACK_CHERRY
    WATCard* watcard = nullptr;
};