#pragma once
#include<string>
#include<iostream>
#include<fstream>

class ThreadPoolLogger
{
public:
	ThreadPoolLogger(std::string Filename);
	~ThreadPoolLogger();
	void Writemessage(std::string message);
private:
	std::string filename;

	std::ofstream out;
};