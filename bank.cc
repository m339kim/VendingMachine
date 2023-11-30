#include "bank.h"
#include <algorithm>

Bank::Bank(unsigned int numStudents) : numStudents(numStudents) {
    balances = new unsigned int[numStudents];
    std::fill(balances, balances + numStudents, 0);
    balanceChange = new uCondition[numStudents];
}

Bank::~Bank() {
    delete [] balances;
}

void Bank::deposit(unsigned int id, unsigned int amount) {
    balances[id] += amount;
    while (!balanceChange[id].empty()) {
        balanceChange[id].signal();
    }
}

void Bank::withdraw(unsigned int id, unsigned int amount) {
    while (balances[id] < amount) {
        // signalBlock instead of signalling all in deposit?
        balanceChange[id].wait();
    }
    balances[id] -= amount;
}