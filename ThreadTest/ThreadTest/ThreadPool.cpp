#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "ThreadPool.h"
#include "LogMessages.h"

TaskWrapper::TaskWrapper(LPTHREAD_START_ROUTINE task)
{
	Task = task;
}

ThreadPool::ThreadPool(int maxThreadCount)
{
	this->logger = new ThreadPoolLogger("./Log.txt");
	InitializeConditionVariable(&conditionvariable);
	InitializeCriticalSection(&criticalsection);
	this->_maxThreadCount = maxThreadCount;
	//this->threadArray = new HANDLE[maxThreadCount];
	try {
		for (int i = 0; i < maxThreadCount; i++)
		{
			//threadArray[i] = CreateThread(NULL, 0, ThreadStart, (void*)this, 0, NULL);
			threads.push_back(CreateThread(NULL, 0, ThreadStart, (void*)this, 0, NULL));
		}
		std::string message = INITIALIZE_MESSAGE + std::to_string(maxThreadCount) + std::to_string(GetCurrentThreadId());
		logger->Writemessage(message);
	}
	catch (...)
	{
		std::string message = THREADPOOL_CREATION_ERROR;
		logger->Writemessage(message);
	}
}

int ThreadPool::ExecuteTask(LPTHREAD_START_ROUTINE func)
{
	if (currentThreadCount < _maxThreadCount)
	{
		EnterCriticalSection(&criticalsection);
		currentThreadCount++;
		wrappers.push_back(new TaskWrapper(func));
		std::string message = TASK_ADDED_MESSAGE + std::to_string(GetCurrentThreadId());
		logger->Writemessage(message);
		LeaveCriticalSection(&criticalsection);
		WakeConditionVariable(&conditionvariable);
		return 1;
	}
	else
	{
		EnterCriticalSection(&criticalsection);
		currentThreadCount++;
		threads.push_back(CreateThread(NULL, 0, ThreadStart, (void*)this, 0, NULL));
		//threadArray[_maxThreadCount] = new HANDLE;
		//threadArray[_maxThreadCount] = CreateThread(NULL, 0, ThreadStart, (void*)this, 0, NULL);
		_maxThreadCount++;
		std::string message = MAX_NUMBER_OF_TASKS + std::to_string(_maxThreadCount);
		logger->Writemessage(message);
		wrappers.push_back(new TaskWrapper(func));
		message = TASK_ADDED_MESSAGE + std::to_string(GetCurrentThreadId());
		logger->Writemessage(message);
		LeaveCriticalSection(&criticalsection);
		WakeConditionVariable(&conditionvariable);
		return 0;
	}
}

DWORD WINAPI ThreadPool::ThreadStart(LPVOID lpParam)
{
	return ((ThreadPool*)lpParam)->ExecutionWrapper();
}

int ThreadPool::ExecutionWrapper()
{
	do
	{
		EnterCriticalSection(&criticalsection);

		while (wrappers.size() == 0 && canExecute)
		{
			SleepConditionVariableCS(&conditionvariable, &criticalsection, INFINITE);
		}

		if (!canExecute)
		{
			LeaveCriticalSection(&criticalsection);
			return 0;
		}

		ExecuteTaskFromVector();
		//LeaveCriticalSection(&criticalsection);

	} while (canExecute);
	return 0;
}

bool ThreadPool::ExecuteTaskFromVector()
{
	TaskWrapper* executing = wrappers.front();
	wrappers.pop_back();
	LeaveCriticalSection(&criticalsection);
	try {
		executing->Task(NULL);
		return true;
		}
	catch(...)
	{
		std::string message = TASK_EXECUTION_ERROR + std::to_string(GetCurrentThreadId());
		logger->Writemessage(message);
	}
}

ThreadPool::~ThreadPool() {

	canExecute = false;
	WakeAllConditionVariable(&conditionvariable);
	//WaitForMultipleObjects(currentThreadCount, threadArray, TRUE, INFINITE);
	for (int i = 0; i < currentThreadCount; i++)
	{
		WaitForSingleObject(threads[i],INFINITE);
	}
	DeleteCriticalSection(&criticalsection);
	/*for (int i = 0; i < this->currentThreadCount; i++) {
		CloseHandle(threadArray[i]);
	}*/
	for (int i = 0; i < currentThreadCount; i++)
	{
		CloseHandle(threads[i]);
	}

}