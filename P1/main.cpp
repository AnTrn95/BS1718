/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Hung
 *
 * Created on 20. November 2017, 08:46
 */

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cctype>
#include <string>
#include <cstring>
#include <unistd.h>
#include <chrono>
#include <time.h>
#include <ctime>  // clock((), CLOCK_PER_SEC
#include <signal.h>  //signal()
#include <sys/wait.h>  //wait()
#include <sys/types.h> //pipe()

using namespace std;

bool IS_AUTO = false; // per default: DEBUG
int CLICK = 1; //clock

void step() { // 1 STEP
    cout << "STEP " << CLICK << endl;
    CLICK++;
}

void stepHandler(int i) {
    if (IS_AUTO) {
        step();
        alarm(1); // resets alarm
    }
}

int main() {
    pid_t nSimulationsManager; //child
    int childStatus;
    const int BUFFERSIZE = 256;
    char command[BUFFERSIZE];
    string cmd = "D";
    string input_submenu = "";
    /*Timer*/
    time_t now = time(0);
    time_t start = time(0);
    char* dt = ctime(&now);

    /*create pipe*/
    int fd[2];
    pipe(fd);

    if ((nSimulationsManager = fork()) == -1) {
        cerr << "Can't fork\n";
        exit(1);
    } else if (nSimulationsManager == 0) { //child Process
        signal(SIGALRM, stepHandler);
        while (1) {
            read(fd[0], command, BUFFERSIZE);
            input_submenu = command;

            // switched zwischen DEBUG und AUTO
            if (input_submenu == "M" || input_submenu == "Mode") {

                if (!IS_AUTO) {
                    IS_AUTO = true;
                    cout << "=== SWITCHED TO AUTO MODE ==\n";

                    alarm(1); // sets first alarm          
                } else {
                    IS_AUTO = false;
                    cout << "=== SWITCHED TO DEBUG MODE ==\n";
                }
            } else if (input_submenu == "S" || input_submenu == "Step") {
                if (!IS_AUTO) step();

            } else if (input_submenu == "P" || input_submenu == "Print") {
                pid_t nReporterChild = fork();

                if (nReporterChild == -1) {
                    cerr << "Cant fork Reporter\n";
                    exit(0);
                } else if (nReporterChild == 0) {
                    time_t end = time(0);
                    cout << "Duration time: " << end-start << endl;
                    exit(0);
                } else {
                    wait(&childStatus);
                    exit(0);
                }
            }//End-nReporterchild

            //exit(0);
        }//End- while(getline())
        cout << cmd << endl;
        cmd.clear();

    } else { //parent process

        string buffer;
        while (1) {
            cout << "Insert command: ";
            cin.getline(command, BUFFERSIZE);

            buffer = command;

            if (buffer == "Q" || buffer == "Quit") {
                // wait(&childStatus);
                return 0;
            }

            /*writing to pipe*/
            write(fd[1], command, BUFFERSIZE);
        }
    }
    return childStatus;

    return 0;
}
