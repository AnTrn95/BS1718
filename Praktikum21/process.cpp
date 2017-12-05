/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   process.cpp
 * Author: debian
 * 
 * Created on 23. November 2017, 23:11
 */

#include "process.h"
int process::counter=0;
process::process() {
    status = bereit;
    id=counter;
    starttime=0;
    counter++;
    ppc=0;
    pr0=0;
}

process::process(const process& orig) {
}

process::~process() {
}

