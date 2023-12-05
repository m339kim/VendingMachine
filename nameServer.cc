#include "nameServer.h"

NameServer::NameServer(Printer& prt, unsigned int numVendingMachines,
                       unsigned int numStudents)
    : printer(prt),
      numVendingMachines(numVendingMachines),
      numStudents(numStudents) {
    machines = new VendingMachine*[numVendingMachines];
}

NameServer::~NameServer() {
    delete[] machines;
}

void NameServer::main() {
    printer.print(Printer::NameServer, States::Start);

    for (unsigned int i = machineIndex; i < numVendingMachines; i++) {
        _Accept(VMregister){  
            machines[i] = newMachine; 
        };
    }
    machineIndex = 0;
    for (;;) {
        _Accept(~NameServer) { 
            bench.signalBlock();
            printer.print(Printer::Kind::NameServer, NameServer::States::Finished);
            break; 
        }
        or _Accept(getMachine) {
            newMachine = machines[machineIndex];
            printer.print(Printer::Kind::NameServer, 
                            NameServer::States::NewVM, 
                            studentId, 
                            newMachine->getId()
                         );
            machineIndex = (machineIndex + 1) % numVendingMachines;
            bench.signalBlock();
        } or _Accept (getMachineList) {}
    }
}

void NameServer::VMregister(VendingMachine* vendingmachine) {
    newMachine = vendingmachine;
    printer.print(Printer::Kind::NameServer, RegisterVM, newMachine->getId());
}

VendingMachine * NameServer::getMachine(unsigned int id) {
    studentId = id;
    bench.wait();
    return newMachine;
}

VendingMachine ** NameServer::getMachineList() {
    return machines;
}
