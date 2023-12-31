#pragma once

_Monitor Bank {
	unsigned int numStudents;
	unsigned int* balances; // of students

    public:
        Bank( unsigned int numStudents );
        ~Bank();
        void deposit( unsigned int id, unsigned int amount );
        void withdraw( unsigned int id, unsigned int amount );
};