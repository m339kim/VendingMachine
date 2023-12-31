#include "WATCardOffice.h"

#include "bank.h"
#include "printer.h"

/*
    _Task Courier
*/
WATCardOffice::Courier::Courier(Printer& printer, Bank& bank,
                                WATCardOffice& office, unsigned int cid)
    : printer(printer), bank(bank), office(office), cid(cid) {}

WATCardOffice::Courier::~Courier() {
    printer.print(Printer::Kind::Courier, cid,
                  WATCardOffice::Courier::States::Finished);
}

void WATCardOffice::Courier::main() {
    printer.print(Printer::Kind::Courier, cid,
                  WATCardOffice::Courier::States::Start);

    for (;;) {
        WATCardOffice::Job* job = office.requestWork();
        if (job == NULL) _Accept(~Courier) {
                break;
            }

        // get $ from bank
        printer.print(Printer::Kind::Courier, cid,
                      WATCardOffice::Courier::States::StartTransfer, job->sid,
                      job->amount);
        bank.withdraw(job->sid, job->amount);

        // deposit that to WATCard
        printer.print(Printer::Kind::Courier, cid,
                      WATCardOffice::Courier::States::DoneTransfer, job->sid,
                      job->amount);
        job->watcard->deposit(job->amount);

        // and while doing that, courier loses watcard by 1/6 chance
        if (my_prng(6) == 0) {
            printer.print(Printer::Kind::Courier, cid,
                          WATCardOffice::Courier::States::WATCardLost,
                          job->sid);
            job->result.exception(new WATCardOffice::Lost());
            delete job->watcard;
        } else {
            job->result.delivery(job->watcard);
        }

        delete job;
    }
}

/*
    _Task WATCardOffice
*/
WATCardOffice::WATCardOffice(Printer& prt, Bank& bank, unsigned int numCouriers)
    : printer(prt),
      bank(bank),
      numCouriers(numCouriers),
      couriers(new Courier*[numCouriers]) {
    // init coureirs
    for (unsigned int i = 0; i < numCouriers; i++) {
        couriers[i] = new Courier(printer, bank, *this, i);
    }
}

WATCardOffice::~WATCardOffice() {
    // delete remaining jobs
    for (size_t i = 0; i < jobs.size(); ++i) {
        Job* job = jobs[i];
        delete job;
    }
    jobs.clear();

    for (unsigned int i = 0; i < numCouriers; i++) _Accept(requestWork);
    for (unsigned int i = 0; i < numCouriers; i++) delete couriers[i];
    delete[] couriers;

    printer.print(Printer::Kind::WATCardOffice,
                  WATCardOffice::States::Finished);
}

void WATCardOffice::main() {
    printer.print(Printer::Kind::WATCardOffice, WATCardOffice::Start);

    for (;;) {
        _Accept(~WATCardOffice) { break; }
        or _Accept(create) {
            WATCard* watcard =
                new WATCard();  // student must remember to free this memory
            // obtain funding from bank
            Job* job = new Job(currSid, currAmount, watcard);
            jobs.push_back(job);
            printer.print(Printer::Kind::WATCardOffice,
                          WATCardOffice::States::CreateComplete, currSid, currAmount);
            bench.signalBlock();
        } or
        _Accept(transfer) {
            Job* job = new Job(currSid, currAmount, currWATCard);
            jobs.push_back(job);
            printer.print(Printer::Kind::WATCardOffice,
                          WATCardOffice::States::TransferComplete, currSid, currAmount);
            bench.signalBlock();
        }
        or _When(!jobs.empty()) _Accept(requestWork) {
            printer.print(Printer::Kind::WATCardOffice,
                          WATCardOffice::RequestComplete);
        }  // Accept
    }
}

WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
    currSid = sid;
    currAmount = amount;
    bench.wait();
    return jobs.back()->result;
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount,
                                          WATCard* card) {
    currSid = sid;
    currAmount = amount;
    currWATCard = card;
    bench.wait();
    return jobs.back()->result;
}

WATCardOffice::Job* WATCardOffice::requestWork() {
    if (jobs.empty())
        return nullptr;  // If nullptr is returned, it means everything is
                         // tearing down.

    Job* job = jobs.back();  // last elem of vector jobs
    jobs.pop_back();         // remove the last elem of vector jobs

    return job;
}
