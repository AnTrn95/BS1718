//#include <string.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <sys/wait.h>
//
//
///*
// * 
// */
//
//
//static volatile sig_atomic_t globalFlag = 0;
//
//void signal_handler(int signal) {
//    globalFlag = 1;
//    return;
//}
//
//enum ausfuehrbare_modien {
//    debug,
//    automatic
//};
//
//
//int main(int argc, char** argv) {
//
//    //Prozessmanagervariablen
//    int modus = debug;
//    char rarray[255];
//    int systemzeit=0;
//    //--
//
//    int status = 0;
//    char warray[255];
//    int fd[2];
//    pipe(fd); //0 = Lesen , 1= Schreiben
//
//
//    if (pipe < 0) {
//        printf("Fehler beim erstellen der Pipe");
//        return 0;
//    }
//
//
//    printf("\n Programm bereit zur eingabe.\n");
//
//    int childPid = fork();
//
//
//
//
//
//    while (1) {
//
//        if (childPid == -1) {
//            printf(" fehler beim erstellen des Kindporzesses \n");
//            exit(-1);
//        }
//
//
//        if (childPid == 0) {
//            signal(SIGUSR1, signal_handler);
//
//
//            if (modus == debug || globalFlag == 1) {
//                close(fd[1]); //Schreibseite der Pipe schließen
//                int nbytes = read(fd[0], rarray, sizeof (rarray));
//
//	
//            if (nbytes >= 1) {
//                printf("Received string: %s\n", rarray);
//                printf("ausgabe von child\n");
//
//                // AUSWERTUNG
//                if (strcmp(rarray, "s") == 0 || strcmp(rarray, "step") == 0) {
//                    systemzeit++;
//                }
//
//                else if (strcmp(rarray, "p") == 0 || strcmp(rarray, "print") == 0) {
//                    int childPid2 = fork();
//
//
//                    if (childPid2 == -1) {
//                        printf("Can´t Report");
//                        exit(1);
//                    } else if (childPid2 == 0) {
//                        int tmp = systemzeit;
//                        printf("Aktuelle Systemzeit: %d\n", tmp);
//                        exit(1);
//                    }
//                }
//                else if (strcmp(rarray, "m") == 0 || strcmp(rarray, "mode") == 0) {
//
//
//                    // mode=0 = Debug mode=1 = Automatic
//
//                    if (modus == debug)
//                        modus = automatic;
//                    else
//                        modus = debug;
//
//                }
//                else if (strcmp(rarray, "q") == 0 || strcmp(rarray, "quit") == 0)
//                    exit(0);
//            }
//                globalFlag = 0;
//            }
//            if (modus == automatic) {
//                printf(".");
//                systemzeit++;
//                sleep(1);
//            }
//        }
//
//        if (childPid > 0) {
//
//            close(fd[0]); //Leseseite der Pipe schließen
//            scanf("%254s", &warray[0]);
//            write(fd[1], warray, sizeof (warray));
//            kill(childPid, SIGUSR1);
//            if (strcmp(warray, "q") == 0 || strcmp(warray, "quit") == 0) {
//                wait(&status);
//                break;
//            } //wenn exit geschrieben wurde auf Kindprozess beendigung warten                
//
//        }
//
//        fflush(stdout);
//        fflush(stdin);
//    }
//    printf("\n BYE \n ");
//    return 0;
//}

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
#include <queue>


/*
 * 
 */
int pc = 0;
int r0 = 0; // Die zwei Register
int systemzeit = 0;
vector<process*> ptabelle;
queue <process*> qtabelle;
queue<process*> blockqueue;
vector<process*>copyqueue;
int quantum =5;

void init() {

    process *init = new process();
    init->status = laufend;
    init->dateiname = "init";
    ptabelle.push_back(init);

}

process* chooseAKT() {
    for (int i = 0; i < ptabelle.size(); i++) {

        if (ptabelle[i]->status == laufend)
            return (ptabelle[i]);
    }
    int tmp;
    if (qtabelle.size() > 0) {
        for(int i=0;i<qtabelle.size();i++){
            copyqueue.push_back(qtabelle[i]);
        }
        
        for(int i=0;i<copyqueue.size();i++){
            if(copyqueue[i]->ppc%quantum==0){
                tmp=i;
            break;}
            
        }
        qtabelle.swap(qtabelle.front()qtabelle[tmp])
        
        qtabelle.front()->status = laufend;
        qtabelle.pop();
    }
    return NULL;
}

void scheduler(int r0, int pc) {
    
    process *AKT = chooseAKT();
    if(AKT==NULL)return;    //kein laufender/bereiter Prozess gefunden
    
    r0=AKT->pr0;    // LADEN
    pc=AKT->ppc;
    
    ifstream load;
    string zmemory;
    

    process *p1;
    load.open(AKT->dateiname, ios::in);
    
        for (int i = 0; i <= AKT->ppc; i++)
            getline(load, zmemory);
    
    if(zmemory=="")return;  //abfangen wenn ende der datei erreicht ist;
        switch (zmemory[0]) {
            case 'B': AKT->status=blockend;blockqueue.push(AKT);break;
            case 'R':
                p1 = new process();
                p1->pid = AKT->id;
                p1->starttime = systemzeit;
                p1->dateiname = zmemory.substr(2, 8);
                ptabelle.push_back(p1);
                qtabelle.push(p1);
                break;
            case 'D': r0=r0-atoi(zmemory.substr(2,2).c_str());break;
            case 'S': r0=   atoi(zmemory.substr(2,2).c_str());break;
            case 'A': r0=r0+atoi(zmemory.substr(2,2).c_str());break;
            case 'E': AKT->status = beendet;
                break;

            default:cout << "Falscher Befehl !";
                break;
        }
        pc++;
        //AKT->ppc++;
        AKT->ppc=pc;
        AKT->pr0=r0;        //SPEICHERN 
    






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
                    if(blockqueue.size()>0){
                   blockqueue.front()->status=bereit;
                   qtabelle.push(blockqueue.front());
                   blockqueue.pop();}
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
