#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <vector>
#include "ThreadPool.h"

TaskWrapper::TaskWrapper(LPTHREAD_START_ROUTINE task)
{
	Task = task;
}

ThreadPool::ThreadPool(int maxThreadCount)
{
	InitializeConditionVariable(&conditionvariable);
	InitializeCriticalSection(&criticalsection);
	this->_maxThreadCount = maxThreadCount;
	this->threadArray = new HANDLE[maxThreadCount];
	for (int i = 0; i < maxThreadCount; i++)
	{
		threadArray[i] = CreateThread(NULL, 0, ThreadStart, (void*)this, 0, NULL);
	}
}

int ThreadPool::ExecuteTask(LPTHREAD_START_ROUTINE func)
{
	if (currentThreadCount < _maxThreadCount)
	{
		EnterCriticalSection(&criticalsection);
		currentThreadCount++;
		wrappers.push_back(new TaskWrapper(func));
		LeaveCriticalSection(&criticalsection);
		WakeConditionVariable(&conditionvariable);
		return 1;
	}
	else
	{
		return 0;
	}
}

DWORD WINAPI ThreadPool::ThreadStart(LPVOID lpParam)
{
	((ThreadPool*)lpParam)->ExecutionWrapper();
	return NULL;
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
		LeaveCriticalSection(&criticalsection);

	} while (canExecute);
	return 0;
}

bool ThreadPool::ExecuteTaskFromVector()
{
	TaskWrapper* executing = wrappers.front();
	executing->Task(NULL);
	wrappers.pop_back();
	return true;
}

ThreadPool::~ThreadPool() {

	canExecute = false;
	WakeAllConditionVariable(&conditionvariable);
	WaitForMultipleObjects(currentThreadCount, threadArray, TRUE, INFINITE);
	DeleteCriticalSection(&criticalsection);
	for (int i = 0; i < this->currentThreadCount; i++) {
		CloseHandle(threadArray[i]);
	}

}