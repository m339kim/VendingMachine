#pragma once

_Monitor Printer {
   public:
    enum Kind {
        Parent,
        Groupoff,
        WATCardOffice,
        NameServer,
        Truck,
        BottlingPlant,
        Student,
        Vending,
        Courier
    };
    Printer(unsigned int numStudents, unsigned int numVendingMachines,
            unsigned int numCouriers);
    void print(Kind kind, char state);
    void print(Kind kind, char state, unsigned int value1);
    void print(Kind kind, char state, unsigned int value1, unsigned int value2);
    void print(Kind kind, unsigned int lid, char state);
    void print(Kind kind, unsigned int lid, char state, unsigned int value1);
    void print(Kind kind, unsigned int lid, char state, unsigned int value1,
               unsigned int value2);
    ~Printer();

   private:
    // get column in buffer information should be depending on its kind
    struct BufferEntry {
        char state;
        unsigned int value1;
        unsigned int value2;
        unsigned int numValues;
    };
    BufferEntry *buffer;
    unsigned int numStudents;
    unsigned int numVendingMachines;
    unsigned int numCouriers;
    unsigned int totalLength;
    int getPosition(Kind kind, unsigned int lid);
    void flush();
    void innerPrint(Kind kind, unsigned int lid, char state,
                    unsigned int value1, unsigned int value2,
                    unsigned int numValues);
};
