#include <windows.h>
#include <tchar.h>
#include "game.h"

HBITMAP hBoard;
HBITMAP hBlack;
HBITMAP hWhite;

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
LRESULT CALLBACK WndProc(HWND hwnd,
                         UINT msg,
                         WPARAM wParam,
                         LPARAM lParam)
{
    switch (msg)
    {
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
        if(currentPlayer == 1)
        {
             SelectObject(memDC, hBlack);
        }
        else
        {
             SelectObject(memDC, hWhite);
        }

        BitBlt(hdc,
               292,87,
               32,32,
               memDC,
               0, 0,
               SRCCOPY);
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
        return 0;
    }
    }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
// 入口函数
{
    // 注册窗口类 窗口类是一个结构体，包含了窗口的属性和行为。我们需要注册一个窗口类，以便创建窗口。
    WNDCLASS wc = {};

    wc.lpfnWndProc = WndProc;                // 谁来处理窗口消息
    wc.hInstance = hInstance;                // 当前应用程序的实例句柄
    wc.lpszClassName = TEXT("GomokuWindow"); // 窗口类的名称

    RegisterClass(&wc);

    // 创建窗口
    HWND hwnd = CreateWindow(
        TEXT("GomokuWindow"),
        TEXT("五子棋"),
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
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