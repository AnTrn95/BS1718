/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimulatedProcess.h
 * Author: debian
 *
 * Created on 23. November 2017, 21:09
 */

#ifndef SIMULATEDPROCESS_H
#define SIMULATEDPROCESS_H

#include<ctime>
#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<list>
#include<vector>

using std::vector;
using std::list;
using std::string;
using std::cout;

class SimulatedProcess
{
public:
	friend std::ostream& operator<<(std::ostream &os, const SimulatedProcess& obj);
	static unsigned int ID_COUNTER;
	SimulatedProcess(){};
	SimulatedProcess(const size_t& ppid, const string& file_name);
	SimulatedProcess(const size_t& ppid, const int& prio, const string& file_name);
	time_t getStartingTime() const;
	void  block();
	void unblock();
	double get_cpu_time()const;
	bool getIsBlocked()const{ return !m_isActive; }
	void setIntegerRegsiter(const int& value){ m_integer_register = value; }
	void setProgramCounter(const int& value){ m_programcounter = value; }
	int getIntegerRegister() const { return m_integer_register; }
	vector<string> getInstructionMemory()const{ return instruction_memory; }
	~SimulatedProcess(){};
private:
	vector<string> instruction_memory;
	int m_prio;
	int m_integer_register, m_programcounter;
	time_t m_starting_time;
	size_t m_pid, m_ppid;
	bool m_isActive = true;

};

#endif 
