/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReaderWriterThreads.h
 * Author: debian
 *
 * Created on 7. Dezember 2017, 20:49
 */

#ifndef READERWRITERTHREADS_H
#define READERWRITERTHREADS_H

#include <semaphore.h>
#include <mutex>
						//preserves right order
extern int fair_rc;								//reader counter

using std::mutex;
/* READER WRITER PROBLEM: reader preference
=========================================*/

// The writer threads
void writer(int writerID, int numSeconds);

// The reader thread
void reader(int readerID, int numSeconds);


/* READER WRITER PROBLEM: fair treatment
=========================================*/

// The writer thread
void fairWriter(int writerID, int numSeconds);

// The reader thread
void fairReader(int readerID, int numSeconds);



#endif /* READERWRITERTHREADS_H */

