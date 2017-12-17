#include <string.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include "process.h"
#include <vector>
#include <deque>


/*
 * 
 */
int pc = 0;
int r0 = 0; // Die zwei Register
int systemzeit = 0;
vector<process*> ptabelle;
deque <process*> qtabelle;
deque<process*> blockqueue;
vector<process*>copyqueue;
int quantum = 5;

void init() {

    process *init = new process();
    init->status = laufend;
    init->dateiname = "init";
    ptabelle.push_back(init);

}

process* chooseAKT() {

    if (qtabelle.empty())return nullptr;
    else {
        qtabelle.front()->m_bursttime= qtabelle.size();
        return qtabelle.front();
    }
}

void scheduler(int r0, int pc) {

    process *AKT = chooseAKT();
    if (AKT == nullptr)return; //kein laufender/bereiter Prozess gefunden
    /*process shorter than time-quantum*/
    else if(systemzeit%quantum <= quantum && AKT->m_bursttime == 0){
        qtabelle.pop_front();
    } /*process longer than time-quantum*/
    else if(systemzeit%quantum <= quantum && AKT->m_bursttime > quantum){
        AKT->m_bursttime = AKT->m_bursttime - quantum;
        
        /*process.front() waiting for the next round at the end*/
        process* remain_bursttime= qtabelle.front();
        qtabelle.push_back(remain_bursttime);
    }

        r0 = AKT->pr0; // LADEN
        pc = AKT->ppc;

        ifstream load;
        string zmemory;


        process *p1;
        load.open(AKT->dateiname, ios::in);

        for (int i = 0; i <= AKT->ppc; i++)
            getline(load, zmemory);

        if (zmemory == "")return; //abfangen wenn ende der datei erreicht ist;
        switch (zmemory[0]) {
            case 'B': AKT->status = blockend;
                blockqueue.push_back(AKT);
                break;
            case 'R':
                p1 = new process();
                p1->pid = AKT->id;
                p1->starttime = systemzeit;
                
                /*bursttime*/
                p1->m_bursttime= AKT->m_bursttime;
                p1->dateiname = zmemory.substr(2, 8);
                ptabelle.push_back(p1);
                qtabelle.push_back(p1);
                break;
            case 'D': r0 = r0 - atoi(zmemory.substr(2, 2).c_str());
                break;
            case 'S': r0 = atoi(zmemory.substr(2, 2).c_str());
                break;
            case 'A': r0 = r0 + atoi(zmemory.substr(2, 2).c_str());
                break;
            case 'E': AKT->status = beendet;
                break;

            default:cout << "Falscher Befehl !";
                break;
        }
        pc++;
        //AKT->ppc++;
        AKT->ppc = pc;
        AKT->pr0 = r0; //SPEICHERN 
        AKT->m_bursttime--;  // decreased bursttime
}

void signal_handler(int signal) {
    scheduler(r0, pc);
    systemzeit++;
    printf(".");
    alarm(1);
    return;
}

enum modien {
    debug,
    automatic
};

int main(int argc, char** argv) {
    init();
    //Prozessmanagervariablen
    int modus = debug;
    char rarray[255];
    int fd[2];
    //--


    int status = 0;
    char warray[255];
    // int fd[2];
    pipe(fd); //0 = Lesen , 1= Schreiben


    if (pipe < 0) {
        printf("Fehler beim erstellen der Pipe");
        return 0;
    }


    printf("\n Programm bereit zur eingabe.\n");

    int childPid = fork();





    while (true) {

        if (childPid == -1) {
            printf(" fehler beim erstellen des Kindporzesses \n");
            exit(-1);
        }


        if (childPid == 0) {
            //*******************************************
            //          Simulationsmanager(Child)   CPU    *
            //*******************************************
            signal(SIGALRM, signal_handler);

            //schedulern

            int retval = fcntl(fd[0], F_SETFL, fcntl(fd[0], F_GETFL) | O_NONBLOCK);
            close(fd[1]); //Schreibseite der Pipe schließen
            int nbytes = read(fd[0], rarray, sizeof (rarray));

            if (nbytes >= 1) {
                printf("Received string: %s\n", rarray);
                printf("ausgabe von child\n");

                // AUSWERTUNG
                if (strcmp(rarray, "s") == 0 || strcmp(rarray, "step") == 0) {
                    scheduler(r0, pc);
                    systemzeit++;
                }


                if (strcmp(rarray, "u") == 0 || strcmp(rarray, "unblock") == 0) {
                    if (blockqueue.size() > 0) {
                        blockqueue.front()->status = bereit;
                        qtabelle.push_back(blockqueue.front());
                        blockqueue.pop_front();
                    }
                }





                else if (strcmp(rarray, "p") == 0 || strcmp(rarray, "print") == 0) {
                    int childPid2 = fork();


                    if (childPid2 == -1) {
                        printf("Can´t Report");
                        exit(1);
                    } else if (childPid2 == 0) {
                        int tmp = systemzeit;
                        system("clear");
                        system("cls");
                        printf("Aktuelle Systemzeit: %d\n", tmp);

                        cout << endl << "****************************************************************" << endl;
                        cout << "The current system state is as follows:" << endl;
                        cout << "****************************************************************" << endl;
                        cout << "Durchschnittliche Prozesslaufzeit: " << "dplaufzeit" << endl;
                        cout << "RUNNING PROCESS:" << endl;
                        cout << endl << "pid  ppid  value  pc starttime" << endl;
                        for (int i = 0; i < ptabelle.size(); i++) {
                            if (ptabelle[i]->status == laufend) {
                                cout << ptabelle[i]->id << " " << ptabelle[i]->pid << " " << ptabelle[i]->pr0 << " " << ptabelle[i]->ppc << " " << ptabelle[i]->starttime << " " << endl;
                            }

                        }
                        cout << "****************************************************************" << endl;
                        cout << "READY PROCESS:" << endl;
                        cout << endl << "pid  ppid  value  pc starttime" << endl;
                        for (int i = 0; i < ptabelle.size(); i++) {
                            if (ptabelle[i]->status == bereit) {
                                cout << ptabelle[i]->id << " " << ptabelle[i]->pid << " " << ptabelle[i]->pr0 << " " << ptabelle[i]->ppc << " " << ptabelle[i]->starttime << " " << endl;
                            }
                        }
                        cout << "****************************************************************" << endl;
                        cout << "BLOCKED PROCESS:" << endl;
                        cout << endl << "pid  ppid  value  pc starttime" << endl;
                        for (int i = 0; i < ptabelle.size(); i++) {
                            if (ptabelle[i]->status == blockend) {
                                cout << ptabelle[i]->id << " " << ptabelle[i]->pid << " " << ptabelle[i]->pr0 << " " << ptabelle[i]->ppc << " " << ptabelle[i]->starttime << " " << endl;
                            }
                        }
                        cout << "****************************************************************" << endl;
                        cout << "DEAD PROCESS:" << endl;
                        cout << endl << "pid  ppid  value  pc starttime" << endl;
                        for (int i = 0; i < ptabelle.size(); i++) {
                            if (ptabelle[i]->status == beendet) {
                                cout << ptabelle[i]->id << " " << ptabelle[i]->pid << " " << ptabelle[i]->pr0 << " " << ptabelle[i]->ppc << " " << ptabelle[i]->starttime << " " << endl;
                            }

                        }

                        exit(1);
                    }
                } else if (strcmp(rarray, "m") == 0 || strcmp(rarray, "mode") == 0) {


                    // mode=0 = Debug mode=1 = Automatic

                    if (modus == debug) {
                        modus = automatic;
                        printf("automatic modus gestartet \n");
                        alarm(1);
                    } else {
                        modus = debug;
                        printf("debug modus gestartet \n");
                        alarm(0); // resettet alarm (alle)
                    }
                } else if (strcmp(rarray, "q") == 0 || strcmp(rarray, "quit") == 0)
                    exit(0);
            }

        }

        if (childPid > 0) {

            //*******************************************
            //            Kommandant(Parent)            *
            //*******************************************
            close(fd[0]); //Leseseite der Pipe schließen
            scanf("%254s", &warray[0]);
            write(fd[1], warray, sizeof (warray));

            if (strcmp(warray, "q") == 0 || strcmp(warray, "quit") == 0) {
                wait(&status);
                break;
            } //wenn exit geschrieben wurde auf Kindprozess beendigung warten                

        }

        fflush(stdout);
        fflush(stdin);
    }
    printf("\n BYE \n ");
    return 0;
}