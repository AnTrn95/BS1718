#include"ReaderWriterThreads.h"
#include "Database.h"

#include<thread> //thread bib
#include<vector>
#include<iostream>
#include<stdexcept>

using namespace std;


int main(int argc, char** argv)
{
	const size_t NUM_OF_THREADS = 2; //number of writing threads
	const int MAX_TIME = 120;
	
	int r_counter = 1, w_counter = 1; 

	/* READER PREFERENCE
	=================================================================================*/

	/*first test round: 9:1 ratio (r_threads:w_threads)*/

//	vector<thread> r_threads(NUM_OF_THREADS * 9); //reading function
//	vector<thread> w_threads = vector<thread>(NUM_OF_THREADS); //writing function

	/*second test round: 1:1 ratio (r_threads:w_threads)*/

	//vector<thread> r_threads = vector<thread>(NUM_OF_THREADS); //reading function
	//vector<thread> w_threads = vector<thread>(NUM_OF_THREADS); //writing function

	/*third test round: 1:9 ratio (r_threads:w_threads)*/

        
        
        
        /*!!Zeigen!!*/
	vector<thread> r_threads = vector<thread>((NUM_OF_THREADS)); //reading function
	vector<thread> w_threads = vector<thread>((NUM_OF_THREADS* 9)); //writing function


	/*create all threads*/
	//push reader threads into verctor
	for (size_t i = 0; i < r_threads.size(); i++) {
		r_threads[i] = thread(reader, r_counter++, MAX_TIME);
	}

	//push writer threads into verctor
	for (size_t i = 0; i < w_threads.size(); i++) {
		w_threads[i] = thread(writer, w_counter++, MAX_TIME);
	}
	std::cout << "Finished creating all threads..." << std::endl << std::endl;

	// wait for all threads to terminate
	for (std::thread& r_thread : r_threads) {
		r_thread.join();
	}

	for (std::thread& w_thread : w_threads) {
		w_thread.join();
	}

	cout << "All thread executions are completed.\n";
	cout << "\n\n_____________________________________________\n";
	theDatabase.printStatistics();

	/* FAIR TREATMENT: no thread starvation pursued, add FIFO scheduling
	=================================================================================*/
//	int fairR_counter = 1, fairW_counter = 1;
	/*first test round: 9:1 ratio (r_threads:w_threads)*/

//	vector<thread> fairR_threads(NUM_OF_THREADS * 9); //reading function
//	vector<thread> fairW_threads = vector<thread>(NUM_OF_THREADS); //writing function

	/*second test round: 1:1 ratio (r_threads:w_threads)*/

	//vector<thread> fairR_threads = vector<thread>(NUM_OF_THREADS); //reading function
	//vector<thread> fairW_threads = vector<thread>(NUM_OF_THREADS); //writing function

	/*third test round: 1:9 ratio (r_threads:w_threads)*/

        
        
        
        
        
        
        /*!!!Zeigen!!!*/
	//vector<thread> fairR_threads = vector<thread>((NUM_OF_THREADS)); //reading function
	//vector<thread> fairW_threads = vector<thread>((NUM_OF_THREADS* 9)); //writing function

	/*create all threads*/
	//push reader threads into verctor
//	for (size_t i = 0; i < fairR_threads.size(); i++) {
//		fairR_threads[i] = thread(fairReader, fairR_counter++, MAX_TIME);
//	}
//
//	//push writer threads into verctor
//	for (size_t i = 0; i < fairW_threads.size(); i++) {
//		fairW_threads[i] = thread(fairWriter, fairW_counter++, MAX_TIME);
//	}
//	std::cout << "Finished creating all threads..." << std::endl << std::endl;
//
//	// wait for all threads to terminate
//	for (std::thread& r_thread : fairR_threads) {
//		r_thread.join();
//	}
//
//	for (std::thread& w_thread : fairW_threads) {
//		w_thread.join();
//	}
//
//	//Ausgabe der durchschnittlichen Daten
//	cout << "All thread executions are completed.\n";
//	cout << "\n\n_____________________________________________\n";
//	theDatabase.printStatistics();
//	return 0;
}
