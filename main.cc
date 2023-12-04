#include <iostream>

#include "WATCardOffice.h"
#include "bank.h"
#include "bottlingPlant.h"
#include "config.h"
#include "groupoff.h"
#include "nameServer.h"
#include "parent.h"
#include "printer.h"
#include "student.h"
#include "uPRNG.h"
#include "vendingMachine.h"

PRNG my_prng;

using namespace std;

int main(int argc, char *argv[]) {
    const char *defaultConfigFile = "soda.config";
    int processors = 1;
    ConfigParms configParms;
    struct cmd_error {};
    try {
        switch (argc) {
            case 4:
                if (strcmp(argv[3], "d") != 0) {
                    processors = convert(argv[3]);
                    if (code < 0) throw cmd_error();
                }
            case 3:
                if (strcmp(argv[2], "d") != 0) {
                    my_prng.set_seed(convert(argv[2])) if (code <
                                                           0) throw cmd_error();
                }
            case 2:
                processConfigFile(argv[1], configParms);
                break;
            case 1:
                processConfigFile(defaultConfigFile, configParms);
                break;
            default:
                throw cmd_error();
        }
    } catch (...) {
        cerr << "soda [ config-file | 'd' [ seed (> 0) | 'd' [ processors (> "
                "0) | 'd' ] ] ]"
             << endl;
        exit(EXIT_FAILURE);
    }

    Printer printer(configParms.numStudents, configParms.numVendingMachines,
                    configParms.numCouriers);
    Bank bank(configParms.numStudents);
    WATCardOffice watcardOffice(printer, bank, configParms.numCouriers);
    Groupoff groupoff(printer, configParms.numStudents, configParms.sodaCost,
                      configParms.groupoffDelay);
    NameServer nameServer(printer, configParms.numVendingMachines,
                          configParms.numStudents);
    VendingMachine **machines =
        new VendingMachine *[configParms.numVendingMachines];
    for (unsigned int i = 0; i < configParms.numVendingMachines; i++) {
        machines[i] =
            new VendingMachine(printer, nameServer, i, configParms.sodaCost);
    }
    BottlingPlant bottlingPlant(
        printer, nameServer, configParms.numVendingMachines,
        configParms.maxShippedPerFlavour, configParms.maxStockPerFlavour,
        configParms.timeBetweenShipments);
    Student **students = new Student *[configParms.numStudents];
    for (unsigned int i = 0; i < configParms.numStudents; i++) {
        students[i] = new Student(printer, nameServer, watcardOffice, groupoff,
                                  i, configParms.maxPurchases);
    }

    for (unsigned int i = 0; i < configParms.numStudents; i++) {
        delete students[i];
    }
    delete[] students;
    bottlingPlant.~BottlingPlant();
    for (unsigned int i = 0; i < configParms.numVendingMachines; i++) {
        delete machines[i];
    }
    delete[] machines;
}
