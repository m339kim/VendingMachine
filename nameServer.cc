#include "nameServer.h"

NameServer::NameServer(Printer& prt, unsigned int numVendingMachines,
                       unsigned int numStudents)
    : printer(prt),
      numVendingMachines(numVendingMachines),
      numStudents(numStudents) {
    studentIdxs = new unsigned int[numStudents];

    for (unsigned int i = 0; i < numStudents; i++) {
        studentIdxs[i] = i % numVendingMachines;
    }

    machines = new VendingMachine*[numVendingMachines];
}

NameServer::~NameServer() {
    delete[] studentIdxs;
    delete[] machines;
}

void NameServer::main() {
    printer.print(Printer::NameServer, States::Start);

    for (unsigned int i = 0; i < numVendingMachines; i++) {
        _Accept(VMregister){};
    }

    for (;;) {
        _Accept(~NameServer) { break; }
        or _Accept(getMachine || getMachineList) {}
    }

    printer.print(Printer::NameServer, States::Finished);
}

void NameServer::VMregister(VendingMachine* vendingmachine) {
    printer.print(Printer::NameServer, States::RegisterVM, vendingmachine->getId());
    machines[machineIndex] = vendingmachine;
    machineIndex++;
}

VendingMachine* NameServer::getMachine(unsigned int id) {
    VendingMachine *currMachine = machines[studentIdxs[id]];
    printer.print(Printer::NameServer, States::NewVM, id, currMachine->getId());
    studentIdxs[id] = (studentIdxs[id] + 1) % numVendingMachines;
    return currMachine;
}

VendingMachine** NameServer::getMachineList() {
    return machines;
}
