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
#include <signal.h>  //signal()
#include <sys/wait.h>  //wait()
#include <sys/types.h> //pipe()

using namespace std;

/*
 * 
 */

void stepHandler(int) {

}

int main() {
    bool menu = true,
            submenu = true,
            IS_AUTO_MODE = false;
    pid_t nSimulationsManager; //child
    int childStatus;
    const int BUFFERSIZE = 256;
    char command[BUFFERSIZE] = "test";
    string cmd = "D";
    string input_submenu = "";
    int i = 0;
    int j = 0;


    /*clock*/
    int time_step = 0;


    /*create pipe*/
    int fd[2];
    pipe(fd);

    while (menu) {
        if ((nSimulationsManager = fork()) == -1) {
            cerr << "Can't fork\n";
            exit(0);
        } else if (j == 0) { //child Process

            if (cmd == "D" || cmd == "Debug") {
                while (submenu) {
                    cout << "Debug-Mode" << endl;
                    /*Reporter*/
                    // pid_t nReporterChild = fork();
                    //cout << nReporterChild << endl;
                    if (i == -1) {
                        cerr << "Cant fork Reporter\n";
                        exit(0);
                    } else if (i == 0) {
                        cout << "---Submenu---\n"
                                << "[M]ode to Automatic-Mode\n"
                                << "[S]tepping\n"
                                << "[P]rint\n"
                                << "[Q]uit\n"
                                << "Choose please: ";
                        cin >> input_submenu;
                        input_submenu = toupper(input_submenu[0]);

                        /*Submenu*/
                        if (input_submenu == "M" || input_submenu == "Mode") {
                            cout << "Mode changed." << endl;
                            if (!IS_AUTO_MODE) {
                                IS_AUTO_MODE = true;
                                cmd = "Automatic";
                                submenu = false;
                                alarm(1);
                            } else {
                                IS_AUTO_MODE = false;
                            }
                        } else if (input_submenu == "S" || input_submenu == "Step") {

                        } else if (input_submenu == "P" || input_submenu == "Print") {

                        } else if (input_submenu == "Q" || input_submenu == "Quit") {
                            submenu = false;
                        } else {
                            cerr << "Wrong input. Please try again!" << endl;
                            input_submenu.clear();
                            continue;
                        }
                    }//End-nReporterchild
                    time_step = 0;
                    //exit(0);
                }//End- while(getline())
                cout << cmd << endl;
                cmd.clear();
                /*Automatic-Mode*/
            } else if (cmd == "A" && IS_AUTO_MODE == true || cmd == "Automatic" && IS_AUTO_MODE == true) {
                cout << "Auto-Mode" << endl;
            }
        } else { //parent process

            /*writing to pipe*/
            //write(fd[1],,);
            wait(&childStatus);
        }
        return childStatus;
    } //End-while(menu)
    return 0;
} 