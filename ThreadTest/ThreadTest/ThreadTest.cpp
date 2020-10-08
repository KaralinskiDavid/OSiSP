// ThreadTest.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include "ThreadPool.h"

typedef DWORD WINAPI Func(LPVOID lpParam);


DWORD WINAPI MyFunc(LPVOID lpParam)
{
    _tprintf(_T("Hello guys"));
    return 0;
}

int main()
{
    /*DWORD (_stdcall *b)(LPVOID lpParam) = MyFunc;
    LPTHREAD_START_ROUTINE t;
    t = &MyFunc;
    HANDLE thread = CreateThread(NULL, 0, b, NULL, CREATE_SUSPENDED, NULL);*/


    /*TP_CALLBACK_ENVIRON environment;
    PTP_WORK_CALLBACK workcallback = MyFunc;
    PTP_WORK work;
    PTP_POOL threadPool = CreateThreadpool(NULL);
    SetThreadpoolThreadMaximum(threadPool, 5);
    InitializeThreadpoolEnvironment(&environment);
    work = CreateThreadpoolWork(MyFunc, NULL, &environment);
    SubmitThreadpoolWork(work);
    CloseThreadpool(threadPool);
    std::cout << "Hello World!\n";*/
    //HANDLE thread = CreateThread(NULL, 0, &MyFunc, NULL, 0, NULL);


    /*_tprintf(_T("Hello guysSSSSS"));
    ResumeThread(thread);
    WaitForSingleObject(thread, INFINITE);*/

    ThreadPool threadPool;
    threadPool.ExecuteTask(MyFunc);

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
