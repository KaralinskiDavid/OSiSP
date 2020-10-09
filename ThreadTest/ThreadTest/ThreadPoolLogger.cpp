#include "ThreadPoolLogger.h"

ThreadPoolLogger::ThreadPoolLogger(std::string Filename)
{
	filename = Filename;
	out.open(filename, std::ios_base::app);
}

ThreadPoolLogger::~ThreadPoolLogger()
{
	out.close();
}

void ThreadPoolLogger::Writemessage(std::string message)
{
	out << message << std::endl;
}