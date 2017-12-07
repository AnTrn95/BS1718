#include "ReaderWriterThreads.h"
#include "Database.h"

#include <pthread.h>
#include <thread>			// for std::this_thread
#include <stdio.h>
#include <iostream>			// for std::cin, cerr, cout ...
#include <chrono>			// for std::chrono... 

/* READER PREFERENCE
=================================================================================*/
int fair_rc = 0;
mutex fair_mutex;
mutex fair_db;								//access control on database
mutex fifoQueue;	

/*semaphores for reader preference solution*/
mutex mutex_1;
mutex db;
int rc = 0;


// The writer thread
void writer(int writerID, int numSeconds) 
{
	std::cout << "Writer " << writerID << " starting..." << std::endl << std::endl;

	int	tests = 0;
	auto startTime = std::chrono::steady_clock::now();
	std::chrono::seconds maxTime(numSeconds);

	//timeslot not exhausted 
	while ((std::chrono::steady_clock::now() - startTime) < maxTime) 
	{ 
		db.lock(); //writer braucht exklusiven Zugriff
		bool result = theDatabase.write(writerID);
		db.unlock(); //exklusiven Zugriff wieder freigeben

		++tests;

		// sleep a while...
		//int numSeconds2sleep = randomInt(3); // i.e. either 0, 1 or 2 
		int numSeconds2sleep = 1;
		//std::chrono::seconds randomSeconds(numSeconds2sleep);
		std::cout << "WRITER " << writerID << '\n';
		std::cout << "_________________________________\n"
			<< "finished test " << tests << '\n'
			<< "result = " << (result == true ? "correct" : "incorrect") << '\n'
			<< "sleeping " << numSeconds2sleep 
			<< " seconds " << "\n_________________________________\n\n";
		//if (numSeconds2sleep) std::this_thread::sleep_for(randomSeconds);
		std::this_thread::sleep_for(std::chrono::seconds(numSeconds2sleep));
	} // repeat until time used is up

	std::cout << "\n== WRITER " << writerID 
		<< " finished == \n" <<"Returning after: " << tests
		<< " tests." << std::endl << std::endl;

} 

// The reader thread
void reader(int readerID, int numSeconds) {

	std::cout << "Reader " << readerID << " starting..." << std::endl << std::endl;

	int	tests = 0;
	auto startTime = std::chrono::steady_clock::now();
	std::chrono::seconds maxTime(numSeconds);
	while ((std::chrono::steady_clock::now() - startTime) < maxTime) 
	{
		mutex_1.lock(); //exklusiver Zugriff fuer reader counter/rc
		rc++;
		if (rc == 1) db.lock();//shared_mutex_db.lock(); //1.Reader => lock DB
		mutex_1.unlock();

		//Read from database
		bool result = theDatabase.read(readerID);
		++tests;
	
		mutex_1.lock(); //exklusiver Zugriff auf reader counter
		rc--;
		if (rc == 0) db.unlock(); //unlock DB, when last reader thread has finished
		mutex_1.unlock();

		// sleep a while...
		//int numSeconds2sleep = randomInt(3); // i.e. either 0, 1 or 2 
		int numSeconds2sleep = 1;
		//std::chrono::seconds randomSeconds(numSeconds2sleep);
		std::cout << "READER " << readerID << "\n________________________________________\n"
			<< "finished test " << tests << '\n'
			<< "result = " << (result==true ? "correct" : "incorrect" )<< '\n'
			<< "sleeping " << numSeconds2sleep 
			<< " seconds " <<"\n________________________________________\n\n";
		std::this_thread::sleep_for(std::chrono::seconds(numSeconds2sleep));

	} // repeat until time is used up

	std::cout << "\n== READER " << readerID
		<< " finished. ==\nReturning after " << tests
		<< " tests." << std::endl << std::endl;

} 

/* FAIR TREATMENT: no thread starvation pursued, add FIFO scheduling
=================================================================================*/


// The writer thread
void fairWriter(int writerID, int numSeconds)
{
	std::cout << "Fair writer " << writerID << " starting..." << std::endl << std::endl;

	int	tests = 0;
	auto startTime = std::chrono::steady_clock::now();
	std::chrono::seconds maxTime(numSeconds);

	//timeslot not exhausted 
	while ((std::chrono::steady_clock::now() - startTime) < maxTime)
	{
		fifoQueue.lock();	//wait to be served
		fair_db.lock();		//exclusive access to db
		fifoQueue.unlock();

		bool result = theDatabase.write(writerID); //write to db

		fair_db.unlock(); //release exclusive access to db

		++tests;

		// sleep a while...
		//int numSeconds2sleep = randomInt(3); // i.e. either 0, 1 or 2 
		int numSeconds2sleep = 1;
		//std::chrono::seconds randomSeconds(numSeconds2sleep);
		std::cout << "WRITER " << writerID << '\n';
		std::cout << "_________________________________\n"
			<< "finished test " << tests << '\n'
			<< "result = " << (result == true ? "correct" : "incorrect") << '\n'
			<< "sleeping " << numSeconds2sleep
			<< " seconds " << "\n_________________________________\n\n";
		//if (numSeconds2sleep) std::this_thread::sleep_for(randomSeconds);
		std::this_thread::sleep_for(std::chrono::seconds(numSeconds2sleep));
	} // repeat until time used is up

	std::cout << "\n== WRITER " << writerID
		<< " finished == \n" << "Returning after: " << tests
		<< " tests." << std::endl << std::endl;

}

// The reader thread
void fairReader(int readerID, int numSeconds) {

	std::cout << "Fair reader " << readerID << " starting..." << std::endl << std::endl;

	int	tests = 0;
	auto startTime = std::chrono::steady_clock::now();
	std::chrono::seconds maxTime(numSeconds);
	while ((std::chrono::steady_clock::now() - startTime) < maxTime)
	{
		fifoQueue.lock();	//wait in line to be served
		fair_mutex.lock();	//exklusiver Zugriff fuer reader counter/rc
		fair_rc++;
		if (fair_rc == 1) fair_db.lock(); //1.Reader => lock DB
		fair_mutex.unlock();

		//Read from database
		bool result = theDatabase.read(readerID);
		++tests;

		fair_mutex.lock(); //exklusiver Zugriff auf reader counter
		fair_rc--;
		if (fair_rc == 0) fair_db.unlock(); //unlock DB, when last reader thread has finished
		fair_mutex.unlock();

		// sleep a while...
		//int numSeconds2sleep = randomInt(3); // i.e. either 0, 1 or 2 
		int numSeconds2sleep = 1;
		//std::chrono::seconds randomSeconds(numSeconds2sleep);
		std::cout << "READER " << readerID << "\n________________________________________\n"
			<< "finished test " << tests << '\n'
			<< "result = " << (result == true ? "correct" : "incorrect") << '\n'
			<< "sleeping " << numSeconds2sleep
			<< " seconds " << "\n________________________________________\n\n";
		std::this_thread::sleep_for(std::chrono::seconds(numSeconds2sleep));

	} // repeat until time is used up

	std::cout << "\n== READER " << readerID
		<< " finished. ==\nReturning after " << tests
		<< " tests." << std::endl << std::endl;

}