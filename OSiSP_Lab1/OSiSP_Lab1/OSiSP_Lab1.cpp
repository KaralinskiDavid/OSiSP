// OSiSP_Lab1.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "OSiSP_Lab1.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
RECT viewPort;

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
    LoadStringW(hInstance, IDC_OSISPLAB1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OSISPLAB1));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OSISPLAB1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OSISPLAB1);
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
BOOL isDrawing = false;
POINT previousPoint = { 0 };
COLORREF currentColor;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rectangle;
    HDC hdc;

    switch (message)
    {
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
            hdc = BeginPaint(hWnd, &ps);
            HBRUSH myBrash;
            GetClientRect(hWnd, &viewPort);


            //Drawing rectangle
            rectangle.top = viewPort.top+10;
            rectangle.bottom = viewPort.top + 110;
            rectangle.left = viewPort.left + 10;
            rectangle.right = viewPort.left + 250;
            FillRect(hdc, &rectangle, reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));

            //Drawing colorbar
            myBrash = CreateSolidBrush(RGB(200, 30, 255));
            rectangle.top = viewPort.bottom/2+80;
            rectangle.left = viewPort.right - 40;
            rectangle.bottom = viewPort.bottom / 2 + 280;
            rectangle.right = viewPort.right;
            FillRect(hdc, &rectangle, myBrash);
            DeleteObject(myBrash);

            myBrash = CreateSolidBrush(RGB(255, 0, 0));
            rectangle.top = viewPort.bottom / 2 + 100;
            rectangle.left = viewPort.right - 20;
            rectangle.bottom = viewPort.bottom / 2 + 120;
            rectangle.right = viewPort.right;
            FillRect(hdc, &rectangle, myBrash);
            DeleteObject(myBrash);

            myBrash = CreateSolidBrush(RGB(0, 255, 0));
            rectangle.top = viewPort.bottom / 2 + 135;
            rectangle.left = viewPort.right - 20;
            rectangle.bottom = viewPort.bottom / 2 + 155;
            rectangle.right = viewPort.right;
            FillRect(hdc, &rectangle, myBrash);
            DeleteObject(myBrash);

            myBrash = CreateSolidBrush(RGB(0, 0, 255));
            rectangle.top = viewPort.bottom / 2 + 170;
            rectangle.left = viewPort.right - 20;
            rectangle.bottom = viewPort.bottom / 2 + 190;
            rectangle.right = viewPort.right;
            FillRect(hdc, &rectangle, myBrash);
            DeleteObject(myBrash);

            myBrash = CreateSolidBrush(RGB(0, 0, 0));
            rectangle.top = viewPort.bottom / 2 + 205;
            rectangle.left = viewPort.right - 20;
            rectangle.bottom = viewPort.bottom / 2 + 225;
            rectangle.right = viewPort.right;
            FillRect(hdc, &rectangle, myBrash);
            DeleteObject(myBrash);

            myBrash = CreateSolidBrush(RGB(255, 255, 255));
            rectangle.top = viewPort.bottom / 2 + 240;
            rectangle.left = viewPort.right - 20;
            rectangle.bottom = viewPort.bottom / 2 + 260;
            rectangle.right = viewPort.right;
            FillRect(hdc, &rectangle, myBrash);
            DeleteObject(myBrash);

            //Drawing line
            HPEN myPen;
            myPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
            SelectObject(hdc, myPen);
            MoveToEx(hdc, viewPort.right/ 2, viewPort.top, NULL);
            LineTo(hdc, viewPort.right/ 2, viewPort.bottom);
            DeleteObject(myPen);

            //Drawing lomanaya
            int nextPointX = viewPort.left;
            int nextPointY = viewPort.bottom / 2;
            int offset = 10;
            myPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
            SelectObject(hdc, myPen);
            MoveToEx(hdc, nextPointX, nextPointY, NULL);
            while (nextPointX < viewPort.right)
            {
                nextPointX += 20;
                offset *= -1;
                LineTo(hdc, nextPointX, nextPointY + offset);
                MoveToEx(hdc, nextPointX, nextPointY, NULL);
            }


            //Drawing Ellipse
            myBrash = CreateSolidBrush(RGB(0, 255, 0));
            SelectObject(hdc, myBrash);
            Ellipse(hdc, viewPort.right / 2 - 200, viewPort.bottom / 2 - 100, viewPort.right / 2 + 200, viewPort.bottom / 2 + 100);
            DeleteObject(myBrash);

            //Drawing poly
            POINT poly[6];
            BYTE polystyle[6] = {PT_LINETO,PT_LINETO, PT_LINETO, PT_LINETO, PT_LINETO, PT_LINETO };
            MoveToEx(hdc, viewPort.right / 2 + viewPort.right / 4 + 100, viewPort.bottom / 2 - viewPort.bottom / 4 - 100, NULL);
            poly[0].x = viewPort.right / 2 + viewPort.right / 4;
            poly[0].y = viewPort.bottom / 2 - viewPort.bottom / 4;

            poly[1].x = viewPort.right / 2 + viewPort.right / 4 + 100;
            poly[1].y = viewPort.bottom / 2 - viewPort.bottom / 4 + 100;

            poly[2].x = viewPort.right / 2 + viewPort.right / 4 + 200;
            poly[2].y = viewPort.bottom / 2 - viewPort.bottom / 4 + 100;

            poly[3].x = viewPort.right / 2 + viewPort.right / 4 + 300;
            poly[3].y = viewPort.bottom / 2 - viewPort.bottom / 4;

            poly[4].x = viewPort.right / 2 + viewPort.right / 4 + 200;
            poly[4].y = viewPort.bottom / 2 - viewPort.bottom / 4 - 100;

            poly[5].x = viewPort.right / 2 + viewPort.right / 4 + 100;
            poly[5].y = viewPort.bottom / 2 - viewPort.bottom / 4 - 100;

            PolyDraw(hdc, poly, polystyle, 6);

            //Printing text
            LOGFONT font;
            font.lfHeight = 40;
            SetTextColor(hdc, RGB(255, 0, 255));
            TextOutA(hdc, viewPort.left + 20, viewPort.bottom / 2 - viewPort.bottom / 8, "Первая лаба?", 12);





            EndPaint(hWnd, &ps);
        }
        break;
    case WM_LBUTTONDOWN:
        previousPoint.x = LOWORD(lParam);
        previousPoint.y = HIWORD(lParam);
        if (previousPoint.x >= viewPort.right - 40 && (previousPoint.y <= viewPort.bottom / 2 + 280 && previousPoint.y >= viewPort.bottom / 2 + 80))
        {
            hdc = GetDC(hWnd);
            currentColor = GetPixel(hdc, previousPoint.x, previousPoint.y);
            ReleaseDC(hWnd, hdc);
        }
        else
        {
            isDrawing = TRUE;
        }
        break;

    case WM_LBUTTONUP:
        if (isDrawing)
        {
            hdc = GetDC(hWnd);
            MoveToEx(hdc, previousPoint.x, previousPoint.y, NULL);
            HPEN myPen;
            myPen = CreatePen(PS_SOLID, 3, currentColor);
            SelectObject(hdc, myPen);
            LineTo(hdc, LOWORD(lParam), HIWORD(lParam));
            DeleteObject(myPen);
            ReleaseDC(hWnd, hdc);
            isDrawing = FALSE;
        }
        break;

    case WM_MOUSEMOVE:
        if (isDrawing)
        {
            hdc = GetDC(hWnd);
            MoveToEx(hdc, previousPoint.x, previousPoint.y, NULL);
            HPEN myPen;
            myPen = CreatePen(PS_SOLID, 3, currentColor);
            SelectObject(hdc, myPen);
            if (LOWORD(lParam) >= viewPort.right - 40 && (HIWORD(lParam) <= viewPort.bottom / 2 + 280 && HIWORD(lParam) >= viewPort.bottom / 2 + 80))
            {
                LineTo(
                    hdc,
                    previousPoint.x = viewPort.right - 80,
                    previousPoint.y = HIWORD(lParam)
                );
            }
            else
            {
                LineTo
                (
                    hdc,
                    previousPoint.x = LOWORD(lParam),
                    previousPoint.y = HIWORD(lParam)
                );
            }
            DeleteObject(myPen);
            ReleaseDC(hWnd, hdc);
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