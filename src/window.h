#pragma once
#include <windows.h>

// Creates and registers the window class, returns the HWND
// Returns nullptr if something went wrong
HWND createMainWindow(HINSTANCE hInstance);

// The message handler — needs to be accessible to RegisterClass
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
