// PhoneBookUI.cpp : Определяет точку входа для приложения.
//
#define _CRT_SECURE_NO_WARNINGS

#include "framework.h"
#include "PhoneBookUI.h"
#include <CommCtrl.h>
#include <vector>
#include <string>
#include <sstream>
#include "PhoneBookLine.h"

#pragma comment(lib, "ComCtl32.Lib")

#define MAX_LOADSTRING 100
#define loadPhoneBook "?loadPhonebook@@YA?AV?$vector@PAVPhoneBookLine@@V?$allocator@PAVPhoneBookLine@@@std@@@std@@V?$basic_string@_WU?$char_traits@_W@std@@V?$allocator@_W@2@@2@@Z"

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND hMainWindow;
const std::vector<std::wstring> columnsName{ L"Phonenumber", L"Lastname", L"Name", L"Surname", L"Street", L"House", L"Corpus", L"Flat" };


HMODULE hLib = LoadLibrary(L"PhoneBook.dll");
typedef std::vector<PhoneBookLine*>(*PhL)(std::wstring);
typedef std::vector<PhoneBookLine*>(*Srch)(std::wstring, int);
PhL loadDatabase = (PhL)GetProcAddress(hLib, loadPhoneBook);
Srch searchByIndex = (Srch)GetProcAddress(hLib, "");


// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PHONEBOOKUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PHONEBOOKUI));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

HWND CreateListView(HWND hWndParent)
{
    HWND hWndListView;
    INITCOMMONCONTROLSEX icex;
    RECT rcClient;

    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);
    GetClientRect(hWndParent, &rcClient);

    hWndListView = CreateWindowEx(NULL, WC_LISTVIEW, L"", WS_CHILD | LVS_REPORT | LVS_EDITLABELS, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, hWndParent, (HMENU)1, GetModuleHandle(NULL), NULL);
    return hWndListView;
}

void CreateColumns(HWND hWndListView)
{
    RECT windowRect;
    LVCOLUMN lvc;

    GetClientRect(hMainWindow, &windowRect);
    lvc.mask = LVCF_WIDTH | LVCF_TEXT;
    lvc.cx = 150;
    for (int i = 0; i < columnsName.size(); i++) {
        lvc.pszText = const_cast<LPWSTR>(columnsName[i].c_str());
        ListView_InsertColumn(hWndListView, i, &lvc);
    }
}

std::wstring IntToWStr(int x) {
    std::wstringstream ss;
    ss << x;
    std::wstring str = ss.str();
    return str;
}

LPWSTR LPWSTRConvert(const std::wstring& s)
{
    LPWSTR ws = new wchar_t[s.size() + 1];
    copy(s.begin(), s.end(), ws);
    ws[s.size()] = 0;
    return ws;
}

void FillListView(HWND hWndListView, std::vector<PhoneBookLine*> lines)
{
    ListView_DeleteAllItems(hWndListView);
    LVITEM lvI;

    lvI.pszText = LPSTR_TEXTCALLBACK;
    lvI.mask = LVIF_TEXT;
    lvI.iSubItem = 0;
    for (int i = 0; i < lines.size(); i++) {
        lvI.iItem = i;

        ListView_InsertItem(hWndListView, &lvI);

        ListView_SetItemText(hWndListView, i, 0, LPWSTRConvert(lines[i]->phonenumber));
        ListView_SetItemText(hWndListView, i, 1, LPWSTRConvert(lines[i]->lastname));
        ListView_SetItemText(hWndListView, i, 2, LPWSTRConvert(lines[i]->firstname));
        ListView_SetItemText(hWndListView, i, 3, LPWSTRConvert(lines[i]->surname));
        ListView_SetItemText(hWndListView, i, 4, LPWSTRConvert(lines[i]->street));
        ListView_SetItemText(hWndListView, i, 5, LPWSTRConvert(IntToWStr(lines[i]->house)));
        ListView_SetItemText(hWndListView, i, 6, LPWSTRConvert(IntToWStr(lines[i]->corpus)));
        ListView_SetItemText(hWndListView, i, 7, LPWSTRConvert(IntToWStr(lines[i]->flat)));
    }
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PHONEBOOKUI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PHONEBOOKUI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


HWND hWndListView;

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        hWndListView = CreateListView(hWnd);
        CreateColumns(hWndListView);
        FillListView(hWndListView, loadDatabase(L"D:\\Work\\PhoneBook\\Debug\\T2.txt"));
        ShowWindow(hWndListView, SW_SHOWDEFAULT);
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
