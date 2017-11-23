/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SimulatedProcess.cpp
 * Author: debian
 * 
 * Created on 23. November 2017, 21:09
 */

#include "SimulatedProcess.h"
#include<iomanip>
unsigned int SimulatedProcess::ID_COUNTER = 0;

std::ostream& operator<<(std::ostream &os, const SimulatedProcess& obj)
{
	os << "pid" << std::setw(10) << "ppid" << std::setw(10) << "priority" << '\t' << std::setw(10) << "value" << std::setw(20) << "start time" << std::setw(10) << "PC" << std::setw(30) << "CPU time used so far" << '\n';
	os << obj.m_pid << std::setw(10) << obj.m_ppid << std::setw(10) << obj.m_prio << '\t' << std::setw(10) << obj.m_integer_register << std::setw(20) << obj.m_starting_time << std::setw(10) << obj.m_programcounter << std::setw(30) << static_cast<int>(obj.get_cpu_time() - obj.m_starting_time);

	return os;
}

double SimulatedProcess::get_cpu_time() const{
	return (double)clock() / CLOCKS_PER_SEC;
}

SimulatedProcess::SimulatedProcess(const size_t& ppid, const string& file_name)
:m_ppid(ppid), m_pid(ID_COUNTER++)
{
    string tmp = file_name + ".txt";
	std::ifstream file_data(tmp.c_str());
	m_prio = 0;
	m_integer_register = 0;
	m_programcounter = 0;
	m_starting_time = (double)clock() / CLOCKS_PER_SEC;
	string buffer = "", content;
	if (file_data.good()){
            cout << "Data good" << std::endl;
		while (getline(file_data, buffer)){
			instruction_memory.push_back(buffer);
			cout << buffer << '\n';
		}
	}
}
SimulatedProcess::SimulatedProcess(const size_t& ppid, const int& prio, const string& file_name)
:m_ppid(ppid), m_prio(prio), m_pid(ID_COUNTER++)
{
	string tmp = file_name + ".txt";
	std::ifstream file_data(tmp.c_str());
	m_prio = 0;
	m_integer_register = 0;
	m_programcounter = 0;
	m_starting_time = (double)clock() / CLOCKS_PER_SEC;
	string buffer = "", content;
	if (file_data.good()){
		while (getline(file_data, buffer)){
			instruction_memory.push_back(buffer);
			cout << buffer << '\n';
		}
	}
}

time_t SimulatedProcess::getStartingTime() const
{
	return m_starting_time;
}

void SimulatedProcess::block(){
	if (m_isActive){
		m_isActive = false;
		cout << "Simulated process is blocked.\n";
	}
	else cout << "Sorry, simulated process is already blocked.\n";
}

void SimulatedProcess::unblock(){
	if (!m_isActive){
		m_isActive = true;
		cout << "Simulated process is unblocked.\n";
	}
	else cout << "Sorry, simulated process is already unblocked.\n";
}

