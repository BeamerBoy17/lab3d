﻿#include "framework.h"
#include "draw.h"
#include "lab1.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HBITMAP pattern;



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

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB1));

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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    char key;

    static draw_atom_t atoms[] = {
        //head
        {{800, 250,1},move_to},
        {{1000, 250,1},line_to},
        {{1000, 450,1},line_to},
        {{800, 450,1},line_to},
        {{800, 250,1},line_to},
        //body
        {{800, 250,1},move_to},
        {{900, 170,1},line_to},
        {{1000, 250,1},line_to},
        {{820, 235,1},move_to},
        {{820, 160,1},line_to},
        {{840, 160,1},line_to},
        {{840, 220,1},line_to},
        //window
        {{860, 320,1},move_to},
        {{900, 320,1},line_to},
        {{900, 360,1},line_to},
        {{860, 360,1},line_to},
        {{860, 320,1},line_to}, };


    static Figure domik;
    //domik.move(200, 0);
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
    case WM_TIMER:
        RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
    break;
 
    case WM_PAINT: {

        PAINTSTRUCT ps;
        static HDC hdc = BeginPaint(hWnd, &ps);
        HPEN newPen = CreatePen(PS_SOLID, 10, RGB(0, 0, 255));
        HDC hdc_background = CreateCompatibleDC(NULL);
        HDC hdc_smoke = CreateCompatibleDC(NULL);

        HBITMAP wallpaper = (HBITMAP)LoadImage(NULL, L"wallpaper.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//Load the bitmap
        SelectObject(hdc_background, wallpaper); //Put the bitmap into the hdc_x

        HBITMAP smoke1 = (HBITMAP)LoadImage(NULL, L"smoke1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//Load the bitmap
        HBITMAP smoke2 = (HBITMAP)LoadImage(NULL, L"smoke2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//Load the bitmap
        HBITMAP smoke3 = (HBITMAP)LoadImage(NULL, L"smoke3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//Load the bitmap
        HBITMAP smoke4 = (HBITMAP)LoadImage(NULL, L"smoke4.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//Load the bitmap

        HBITMAP sprites[] = { smoke1, smoke2, smoke3, smoke4 };

        RECT rect;
        GetWindowRect(hWnd, &rect);

        domik = Figure(hdc, atoms, 17);
        domik.move(-20,70);
        domik.draw();

        SelectObject(hdc_background, wallpaper);
        while (true)
        {
           for (size_t i = 0; i < 4; i++)
            {
               BitBlt(hdc, 0, 0, 1900, 2000, hdc_background, 0, 0, SRCCOPY);
                domik.draw();
                SelectObject(hdc_smoke, sprites[i]);
                BitBlt(hdc, 790, 70, 1900, 2000, hdc_smoke, 0, 0, SRCPAINT);
                Sleep(600);
           }
        }

        SelectObject(hdc_background, wallpaper);
        BitBlt(hdc, 0, 0, 1900, 2000, hdc_background, 0, 0, SRCCOPY);
        domik.draw();

        SelectObject(hdc_smoke, smoke1);
        BitBlt(hdc, 0, 0, 1900, 2000, hdc_smoke, 0, 0, SRCPAINT);
        Sleep(1000);

        SelectObject(hdc_smoke, smoke1);
        BitBlt(hdc, 0, 0, 1900, 2000, hdc_smoke, 0, 0, SRCPAINT);
        Sleep(1000);

        SelectObject(hdc_smoke, smoke2);
        BitBlt(hdc, 0, 0, 1900, 2000, hdc_smoke, 0, 0, SRCPAINT);
        Sleep(1000);

        SelectObject(hdc_smoke, smoke3);
        BitBlt(hdc, 0, 0, 1900, 2000, hdc_smoke, 0, 0, SRCPAINT);
        Sleep(1000);

        SelectObject(hdc_smoke, smoke4);
        BitBlt(hdc, 0, 0, 1900, 2000, hdc_smoke, 0, 0, SRCPAINT);
        Sleep(1000);



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
