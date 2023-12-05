#include "nameServer.h"

NameServer::NameServer(Printer& prt, unsigned int numVendingMachines,
                       unsigned int numStudents)
    : printer(prt),
      numVendingMachines(numVendingMachines),
      numStudents(numStudents) {
    machines = new VendingMachine*[numVendingMachines];
}

NameServer::~NameServer() { delete[] machines; }

void NameServer::main() {
    printer.print(Printer::NameServer, States::Start);

    for (unsigned int i = machineIndex; i < numVendingMachines; i++) {
        _Accept(VMregister) {
            machines[i] = newMachine;
            printer.print(Printer::Kind::NameServer, RegisterVM,
                          newMachine->getId());
        };
    }
    machineIndex = 0;
    for (;;) {
        _Accept(~NameServer) {
            bench.signalBlock();
            printer.print(Printer::Kind::NameServer,
                          NameServer::States::Finished);
            break;
        }
        or _Accept(getMachineList){} or _Accept(getMachine) {
            printer.print(Printer::Kind::NameServer, NameServer::States::NewVM,
                          studentId, newMachine->getId());
            machineIndex = (machineIndex + 1) % numVendingMachines;
        }
    }
}

void NameServer::VMregister(VendingMachine* vendingmachine) {
    newMachine = vendingmachine;
}

VendingMachine* NameServer::getMachine(unsigned int id) {
    studentId = id;
    newMachine = machines[machineIndex];
    return newMachine;
}

VendingMachine** NameServer::getMachineList() { return machines; }
