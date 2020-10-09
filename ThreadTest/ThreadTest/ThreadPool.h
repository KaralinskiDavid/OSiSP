#pragma once
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <mutex>
#include "ThreadPoolLogger.h"

class TaskWrapper
{
public:
	TaskWrapper(LPTHREAD_START_ROUTINE task);
	LPTHREAD_START_ROUTINE Task;
};

class ThreadPool {
public:
	ThreadPool(int maxThreadCount = 3);
	~ThreadPool();
	int ExecuteTask(LPTHREAD_START_ROUTINE Func);
private:
	int _maxThreadCount;
	int currentThreadCount = 0;

	bool canExecute = true;
	bool hasMaxCount = false;

	HANDLE ThreadManager;
	//static DWORD WINAPI ManageThreads(LPVOID lpParam);
	static DWORD WINAPI ThreadStart(LPVOID lpParam);
	int ExecutionWrapper();
	bool ExecuteTaskFromVector();
	CONDITION_VARIABLE conditionvariable;
	CRITICAL_SECTION criticalsection;

	//HANDLE* threadArray;
	std::vector<TaskWrapper*> wrappers;
	std::vector<HANDLE> threads;
	
	ThreadPoolLogger *logger;
};

