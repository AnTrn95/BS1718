/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   process.h
 * Author: debian
 *
 * Created on 23. November 2017, 23:11
 */
enum state{
   bereit,
    laufend,
    blockend,
    beendet   
};

#include <iostream>
#include <string>
using namespace std;

#ifndef PROCESS_H
#define PROCESS_H

class process {
public:
    process();
    process(const process& orig);
    virtual ~process();
    string dateiname;
    int status; //0 = off 1= on/active
    int id;
    static int counter;
    int pid;
    int m_bursttime;
    int starttime;
    int pr0;
    int ppc;
    

};

#endif /* PROCESS_H */
