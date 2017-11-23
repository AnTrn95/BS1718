/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: debian
 *
 * Created on 23. November 2017, 20:35
 */

#include"SimulatedProcess.h"


#include<ctime> // clock((), CLOCK_PER_SEC
#include<cstdlib>
#include<iostream>
#include<sstream>
#include<iomanip>
#include<list>
#include <unistd.h>
#include <sys/types.h>  //pipe()
#include <signal.h>  //signal()
#include<stdio.h>
#include <wait.h>  //wait()
#include <string>
#include<stdexcept>
using namespace std;

using namespace std;

bool IS_AUTO = false; // per default: DEBUG
int CLICK = 1; //clock

const float QUANTUM = 4; //quantum for ROUND ROBIN SCHEDULING
int PROCESS_STEPS = 0;

//2 queues for scheduling
list<SimulatedProcess> blocked_processes;
list<SimulatedProcess> non_blocked_processes;

//two registers 
int* integer_register = NULL;
int* program_counter = NULL;
int current_int_reg_value = 0;
int pc_value = 0;

SimulatedProcess *currently_simulated_process, buffer; //currently executed simulated process, buffer for process switching
SimulatedProcess sim_process(getppid(), "init"); //initial program: init

int CURRENT_STEP = 0;

//count current steps in total
size_t step_counter = 0;

//for n value in the given command
string file = "";
int value;

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

//increments steps and returns quantum exhaustion state

static bool IS_QUANTUM_OVER() {
    PROCESS_STEPS++;
    if (PROCESS_STEPS >= QUANTUM) {
        PROCESS_STEPS = 0;
        return true;
    }
    return false;
}

void simulatedCPU() {
    //all blocked
    if (currently_simulated_process->getIsBlocked()) { // Parent has to UNBLOCK process
        cout << "Sorry, this simulated process is currently blocked. Unblock it and try again.\n";
        exit(0); //return to parent 
    }

    if (CURRENT_STEP < currently_simulated_process->getInstructionMemory().size()) //executable command line exists
    {
        cout << "Next step. Step " << ++step_counter << endl; //print out current step 
        CURRENT_STEP++;
    }
    string instruction;
    try {
        instruction = currently_simulated_process->getInstructionMemory().at(CURRENT_STEP - 1); //get next command line       
    } catch (const std::out_of_range& o) {
        cout << "blaaaah: " << currently_simulated_process->getInstructionMemory().size() << endl;
    }
    /*extract n from command line*/
    if (instruction[0] != 'E' && instruction[0] != 'B') { //case: no n in command line
        if (toupper(instruction[0]) != 'R') stringstream(instruction.substr(1, instruction.size() - 1)) >> value; // n= integer value
        else stringstream(instruction.substr(2, instruction.size() - 1)) >> file; // n= file name
    }

    /*simulated CPU instruction set*/
    switch (toupper(instruction[0])) {
        case 'S': //Init integer register	
            integer_register = &value;
            break;

        case 'A': //Add to integer register value
            current_int_reg_value += value;
            integer_register = &current_int_reg_value;
            break;

        case 'D': //Subtract from integer register value
            current_int_reg_value -= value;
            integer_register = &current_int_reg_value;
            break;

        case 'B': //Block simulated process
            currently_simulated_process->block(); // switch process state to blocked
            blocked_processes.push_back(*currently_simulated_process); //add to blocked processes queue

            if (!non_blocked_processes.empty()) { //there is at least one queued process ready to be executed
                buffer = non_blocked_processes.front(); // scheduling descision: oldest queued process ready to be executed= current sim. process
                currently_simulated_process = &buffer;
                CURRENT_STEP = 0;
                non_blocked_processes.pop_front(); // remove from queue
            } else currently_simulated_process = NULL; //no non-blocked processes available 

            break;

        case 'E': //Beende sim. Prozess
            cout << "Terminated simulated process.\n";
            exit(0);
            break;

        case 'R': //neuer sim.Prozess + Datei ausfuehren
        {
            SimulatedProcess new_process(0, file);
            pc_value++;
            program_counter = &pc_value;

            new_process.setIntegerRegsiter(*integer_register); //speichere Integerregister
            new_process.setProgramCounter(*program_counter); //speichere Programcounter

            non_blocked_processes.push_back(new_process); //aufnehmen in Schlange der nicht blockierten Prozesse	

        }
            break;
        default:
            cout << "Sorry, unknown simulated CPU command. Please try again.\n";
            break;
    }
    cout << *currently_simulated_process << endl;
    cout << instruction << endl;

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

            //read from pipe
            currently_simulated_process = &sim_process; //program start: init is the initially executed program

            //ROUND- ROBIN SCHEDULING: set starting/ reset process timer
            if (IS_QUANTUM_OVER()) // quantum exhausted
            {
                if (!non_blocked_processes.empty()) {

                    non_blocked_processes.push_back(*currently_simulated_process); //current process is queued at the far end of the line 
                    buffer = non_blocked_processes.front();
                    currently_simulated_process = &buffer; //switch with oldest queued (ready-to-execute) process 
                    non_blocked_processes.pop_front(); //remove from queue

                }
            }

            // switched zwischen DEBUG und AUTO
            if (input_submenu == "M" || input_submenu == "Mode") {

                if (!IS_AUTO) {
                    IS_AUTO = true;
                    cout << "=== SWITCHED TO AUTO MODE ==\n";
                    simulatedCPU();
                    alarm(1); // sets first alarm          
                } else {
                    IS_AUTO = false;
                    cout << "=== SWITCHED TO DEBUG MODE ==\n";
                }
            } else if (input_submenu == "S" || input_submenu == "Step") {
                if (!IS_AUTO){ 
                    step();
                    simulatedCPU();
                }

            } else if (input_submenu == "P" || input_submenu == "Print") {
                pid_t nReporterChild = fork();

                if (nReporterChild == -1) {
                    cerr << "Cant fork Reporter\n";
                    exit(0);
                } else if (nReporterChild == 0) {
                    time_t end = time(0);
                    cout << "\n___________________________________________________________________________________________________________\n";
                    cout << "The current system state is as follows :\n";
                    cout << "_____________________________________________________________________________________________________________\n\n";

                    cout << "Duration time: " << end - start << endl;
                    cout << "RUNNING PROCESS:\n";
                    cout << sim_process << '\n';
                    cout << "-------------------------------------------------------------------------------------------------------------\n";
                    cout << "BLOCKED PROCESSES:\n";
                    for (auto& process : blocked_processes) {
                        cout << process;
                    }
                    cout << "-------------------------------------------------------------------------------------------------------------\n";
                    cout << "PROCESSES READY TO EXECUTE:\n";
                    for (auto& process : non_blocked_processes) {
                        cout << process;
                    }
                    cout << endl;
                    exit(0);
                } else {
                    wait(&childStatus);
                    exit(0);
                }
            }//End-nReporterchild
            
            //unblock oldest blocked process: scheduling descision= process switch: current simulated process will be replaced by unblocked process
            else if (cmd == "U" || cmd == "Unblock") {
                blocked_processes.front().unblock(); //change status from blocked to unblock
                non_blocked_processes.push_front(*currently_simulated_process); //switched process will be queued as the next executable process in line

                //delete process from the blocked processes queue
                buffer = blocked_processes.front();
                blocked_processes.pop_front();

                currently_simulated_process = &buffer; //current process equals the unblocked process
            }
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

