#ifndef DATABASE_H
#define DATABASE_H

#include "Stopwatch.h"  
#include <string>		
#include <iostream>		
#include <cstdlib>		// for exit()

class Database
{
public:
	// constructor
	Database()
		: everything_ok(true) // until something goes wrong...
	{
		if (0 < numberDatabases) {	// Singleton check
			std::cerr << "There can only be one database!!!" << std::endl;
			exit(1);
		};
	} // end constructor 

	// Key Methods
	bool write(int writerID); // returns true iff operation ok
	bool read(int readerID); // returns true iff operation ok

	bool ok() const { return everything_ok; }

	void printStatistics() const; // output reader and writer times

private:
	static int numberDatabases;
	bool   everything_ok;
	Stopwatch	readerClock;
	Stopwatch	writerClock;

	// Static constants
	static const std::string inputFileName;
	static const std::string outputFileName;

}; // end class database

extern Database	 theDatabase;

int randomInt(int maxRandomInt); // returns 0 <= randomInt < maxRandomInt

#endif