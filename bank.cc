#include "bank.h"
#include <algorithm>

Bank::Bank(unsigned int numStudents) : numStudents(numStudents) {
    balances = new unsigned int[numStudents];
    std::fill(balances, balances + numStudents, 0);
}

Bank::~Bank() {
    delete [] balances;
}

void Bank::deposit(unsigned int id, unsigned int amount) {
    balances[id] += amount;
}

void Bank::withdraw(unsigned int id, unsigned int amount) {
    for (;;) {
        if (balances[id] < amount) _Accept(deposit);
        break;
    }
    balances[id] -= amount;
}