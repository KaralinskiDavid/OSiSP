#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <vector>
#include "ThreadPool.h"

TaskWrapper::TaskWrapper(LPTHREAD_START_ROUTINE task)
{
	Task = task;
}

ThreadPool::ThreadPool()