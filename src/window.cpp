#include "window.h"
#include "./window/gestures.h"
#include "./tray/tray.h"
#include <dwmapi.h>
#include <windef.h>
#include <winuser.h>

static const int MENU_GESTURE_BASE = 2000;
static const int MENU_QUIT         = 1;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_TRAY:
        if (lParam == WM_RBUTTONUP) showTrayMenu(hwnd);
        return 0;

    case WM_COMMAND: {
        int id = LOWORD(wParam);
        if (id == MENU_QUIT) {
            PostQuitMessage(0);
            return 0;
        }
        if (id >= MENU_GESTURE_BASE && id < MENU_GESTURE_BASE + 400) {
            int dirIndex      = (id - MENU_GESTURE_BASE) / 100;
            SnapAction action = (SnapAction)((id - MENU_GESTURE_BASE) % 100);
            switch (dirIndex) {
                case 0: gestureConfig.left  = action; break;
                case 1: gestureConfig.right = action; break;
                case 2: gestureConfig.up    = action; break;
                case 3: gestureConfig.down  = action; break;
            }
        }
        return 0;
    }


    case WM_DESTROY:
        removeTrayIcon();
        removeGestureHook();
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

HWND createMainWindow(HINSTANCE hInstance) {
    WNDCLASS wc      = {};
    wc.hInstance     = hInstance;
    wc.lpfnWndProc   = WindowProc;
    wc.lpszClassName = L"SquareClass";

    if (!RegisterClass(&wc)) return nullptr;

    HWND hwnd = CreateWindowEx(0, L"SquareClass", L"my-wm",
                               WS_OVERLAPPEDWINDOW, 0, 0, 0, 0,
                               nullptr, nullptr, hInstance, nullptr);

    BOOL dark = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));

    return hwnd;
}
