#include <windows.h>
#include "game.h"
#include <tchar.h>
#define UNICODE
#define _UNICODE

HBITMAP hBoard;
HBITMAP hBlack;
HBITMAP hWhite;
HBITMAP habout;

static HBITMAP LoadBitmapFromExeDir(const TCHAR *name)
{
    TCHAR path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);

    TCHAR *slash = _tcsrchr(path, TEXT('\\'));
    slash += 1;
    if (slash != NULL)
    {
        *(slash) = TEXT('\0');
    }

    lstrcat(path, name);
    return (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

int LOCX[16] = {276, 312, 349, 386, 422, 459, 495, 532, 569, 606, 642, 679, 715, 752, 785, 816};
int LOCY[16] = {71, 107, 144, 181, 217, 254, 291, 327, 364, 401, 437, 474, 510, 546, 580, 618};

LRESULT CALLBACK WndProc(HWND hwnd,
                         UINT msg,
                         WPARAM wParam,
                         LPARAM lParam)
{
    switch (msg)
    {
    case WM_SETCURSOR:
    {
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        return TRUE;
    }
    case WM_CREATE:
    {
        hBoard = LoadBitmapFromExeDir(TEXT("board.bmp"));
        hBlack = LoadBitmapFromExeDir(TEXT("black.bmp"));
        hWhite = LoadBitmapFromExeDir(TEXT("white.bmp"));

        return 0;
    }
    case WM_DESTROY:
        if (hBoard)
            DeleteObject(hBoard);
        if (hBlack)
            DeleteObject(hBlack);
        if (hWhite)
            DeleteObject(hWhite);
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        HDC memDC = CreateCompatibleDC(hdc);

        SelectObject(memDC, hBoard);

        BitBlt(hdc,
               0, 0,
               1024, 768,
               memDC,
               0, 0,
               SRCCOPY);

        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (board[i][j] == 1)
                {
                    SelectObject(memDC, hBlack);
                }
                else if (board[i][j] == 2)
                {
                    SelectObject(memDC, hWhite);
                }
                else
                {
                    continue;
                }

                BitBlt(hdc,
                       LOCX[i], LOCY[j],
                       36, 36,
                       memDC,
                       0, 0,
                       SRCCOPY);
            }
        }

        DeleteDC(memDC);

        EndPaint(hwnd, &ps);

        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        int i = ToGridX(x);
        int j = ToGridY(y);

        if (TryPlace(i, j))
        {
            InvalidateRect(hwnd, NULL, TRUE);
        }
        if (x > 36 && y > 150 && x < 197 && y < 185)
        {
            gamestatus = 0;
            for (int i = 0; i < BOARD_SIZE; i++)
            {
                for (int j = 0; j < BOARD_SIZE; j++)
                {
                    board[i][j] = 0;
                }
            }
            currentPlayer = 1;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        if (x > 36 && y > 74 && x < 197 && y < 107)
        {
            CreateWindowW(
                L"PictureWindow", // 窗口类型名字
                L"说明",          // 标题栏文字
                WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
                300, // x
                300, // y
                415, // 宽
                572, // 高
                NULL,
                NULL,
                GetModuleHandle(NULL),
                NULL);
        }
        return 0;
    }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK ChildProc(HWND hwnd,
                           UINT msg,
                           WPARAM wParam,
                           LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        habout = LoadBitmapFromExeDir(TEXT("about.bmp"));
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        HDC memDC = CreateCompatibleDC(hdc);

        SelectObject(memDC, habout);

        BitBlt(hdc,
               0, 0,
               415, 572,
               memDC,
               0, 0,
               SRCCOPY);

        DeleteDC(memDC);

        EndPaint(hwnd, &ps);

        return 0;
    }
    case WM_DESTROY:
        if (habout)
        {
            DeleteObject(habout);
        }
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
// 入口函数
{
    // 注册窗口类 
    WNDCLASS wc = {};

    wc.lpfnWndProc = WndProc;                // 谁来处理窗口消息
    wc.hInstance = hInstance;                // 当前应用程序的实例句柄
    wc.lpszClassName = TEXT("GomokuWindow"); // 窗口类的名称

    RegisterClass(&wc);

    WNDCLASS child = {};

    child.lpfnWndProc = ChildProc;
    child.hInstance = hInstance;
    child.lpszClassName = TEXT("PictureWindow");

    RegisterClass(&child);
    // 创建窗口
    HWND hwnd = CreateWindowExW(
        0,
        L"GomokuWindow",
        L"五子棋",
        WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1024,
        790,
        NULL,
        NULL,
        hInstance,
        NULL);
    if (hwnd == NULL)
    {
        return 0;
    }

    // 显示窗口
    ShowWindow(hwnd, nCmdShow);

    // 消息循环
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}