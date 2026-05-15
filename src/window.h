#pragma once
#include <windows.h>

HWND createMainWindow(HINSTANCE hInstance);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
