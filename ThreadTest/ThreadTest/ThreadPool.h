#pragma once
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <vector>

class TaskWrapper
{
public:
	TaskWrapper(LPTHREAD_START_ROUTINE task);
	LPTHREAD_START_ROUTINE Task;
};

class ThreadPool {
public:
	ThreadPool(int threadCount = 5);
	~ThreadPool();
	int ExecuteTask(LPTHREAD_START_ROUTINE Task);
private:
	DWORD WINAPI TaskExtractor(LPVOID lpParam);
	CRITICAL_SECTION criticalSection;
	CONDITION_VARIABLE conditionVariable;

	HANDLE* threadArray;
	std::vector<TaskWrapper*> wrappers;

	int _maxThreadCount;
	int _currentThreadCount;
	bool _hasTasks;
	bool _canExecute;
	
};

