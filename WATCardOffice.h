#pragma once
#include <queue>
#include "MPRNG.h"
#include "WATCard.h"
#include "Printer.h"
#include "NameServer.h"
#include "Bank.h"

using namespace std;
extern MPRNG mprng;
_Task WATCardOffice {
    struct Job {                                                    // marshalled arguments and return future
        unsigned int sid;
        unsigned int amount;
        WATCard *card;
        WATCard::FWATCard result;                                   // return future
        Job( unsigned int sid, unsigned int amount, WATCard *card ):
            sid(sid),
            amount(amount),
            card(card){}
    };

    _Task Courier {
        Printer &printer;
        Bank &bank;
        int id;
        WATCardOffice *watcardoffice;
        void main(){
            printer.print(Printer::Kind::Courier, id, 'S');
            for(;;){
                Job *job = watcardoffice->requestWork();
                if(job == NULL){
                    break;
                }

                printer.print(Printer::Kind::Courier, id, 't', job->sid, job->amount);       //start
                bank.withdraw(job->sid, job->amount);
                job->card->deposit(job->amount);
                printer.print(Printer::Kind::Courier, id, 'T', job->sid, job->amount);       //end

            if(mprng(5) == 0){                                    //Lost
                printer.print(Printer::Kind::Courier, id, 'L');
                delete job->card;
                job->result.exception(new WATCardOffice::Lost);
            }else{
                job->result.delivery(job->card);
            }
            delete job;
            }
        }
    public:
        Courier(Printer &prt, Bank &bank, int id, WATCardOffice *watcardoffice):                  //constructor
            printer(prt),
            bank(bank),
            id(id),
            watcardoffice(watcardoffice){}
        ~Courier(){
            printer.print(Printer::Kind::Courier, id, 'F');
        }
    };                                                              // communicates with bank

    void main();
    Printer &printer;
    Bank &bank;
    unsigned int numCouriers;
    Courier **couriers;
    queue<Job*> job_queue;
  public:
    _Event Lost {};                           // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};
