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

    try { // process command-line arguments
        switch (argc) {
            case 4:
                if (strcmp(argv[3], "d") != 0) {
                    processors = convert(argv[3]);
                    if (processors < 0) throw cmd_error();
                }
            case 3:
                if (strcmp(argv[2], "d") != 0) {
                    int temp = convert(argv[2]);
                    if (temp < 0) throw cmd_error();
                    my_prng.set_seed(temp);
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

    // set processors
    uProcessor p[processors - 1]; // number of kernel threads
    if ( processors == 1 ) uThisProcessor().setPreemption( 0 ); // turn off time - slicing for reproducibility

    // init
    Printer printer(configParms.numStudents, configParms.numVendingMachines, configParms.numCouriers);
    Bank bank(configParms.numStudents);
    Parent parent(printer, bank, configParms.numStudents, configParms.parentalDelay);
    WATCardOffice watcardOffice(printer, bank, configParms.numCouriers);
    Groupoff groupoff(printer, configParms.numStudents, configParms.sodaCost, configParms.groupoffDelay);
    
    // init productions
    NameServer nameServer(printer, configParms.numVendingMachines, configParms.numStudents);
    VendingMachine ** machines = new VendingMachine *[configParms.numVendingMachines];
    for (unsigned int i = 0; i < configParms.numVendingMachines; i++) {
        machines[i] = new VendingMachine(printer, nameServer, i, configParms.sodaCost);
    }

    BottlingPlant *bottlingPlant = new BottlingPlant(
        printer, nameServer, configParms.numVendingMachines,
        configParms.maxShippedPerFlavour, configParms.maxStockPerFlavour,
        configParms.timeBetweenShipments);
    
    // init students
    Student **students = new Student *[configParms.numStudents];
    for (unsigned int i = 0; i < configParms.numStudents; i++) {
        students[i] = new Student(printer, nameServer, watcardOffice, groupoff,
                                  i, configParms.maxPurchases);
    }
    // delete them
    for (unsigned int i = 0; i < configParms.numStudents; i++) {
        delete students[i];
    }
    delete[] students;

    // delete plant
    delete bottlingPlant;
    for (unsigned int i = 0; i < configParms.numVendingMachines; i++) {
        delete machines[i];
    }
    delete[] machines;
}
