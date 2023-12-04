#include "config.h"
#include "bank.h"
#include "printer.h"
#include "parent.h"
#include "WATCardOffice.h"
#include "groupoff.h"
#include "nameServer.h"
#include "vendingMachine.h"
#include "bottlingPlant.h"
#include "student.h"
#include "uPRNG.h"

PRNG my_prng;

int main(int argc, char * argv[]) {
    const char * configFile = "";
    ConfigParms configParms;
    processConfigFile(configFile, configParms);
    my_prng.set_seed(1);
    Printer printer(configParms.numStudents, configParms.numVendingMachines, configParms.numCouriers);
    Bank bank(configParms.numStudents);
    WATCardOffice watcardOffice(printer, bank, configParms.numCouriers);
    Groupoff groupoff(printer, configParms.numStudents, configParms.sodaCost, configParms.groupoffDelay);
    NameServer nameServer(printer, configParms.numVendingMachines, configParms.numStudents);
    VendingMachine **machines = new VendingMachine*[configParms.numVendingMachines];
    for (unsigned int i = 0; i < configParms.numVendingMachines; i++) {
        machines[i] = new VendingMachine(printer, nameServer, i, configParms.sodaCost);
    }
    BottlingPlant bottlingPlant(printer, nameServer, configParms.numVendingMachines, configParms.maxShippedPerFlavour, configParms.maxStockPerFlavour, configParms.timeBetweenShipments);
    Student **students = new Student*[configParms.numStudents];
    for (unsigned int i = 0; i < configParms.numStudents; i++) {
        students[i] = new Student(printer, nameServer, watcardOffice, groupoff, configParms.maxPurchases);
    }

    for (unsigned int i = 0; i < configParms.numStudents; i++) {
        delete students[i];
    }
    delete [] students;
    delete bottlingPlant;
    for (unsigned int i = 0; i < configParms.numVendingMachines; i++) {
        delete machines[i];
    }
    delete [] machines;
}


