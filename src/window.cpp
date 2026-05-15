#include "window.h"
#include "./window/gestures.h"
#include <windef.h>
#include <winuser.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
  case WM_DESTROY:
    removeGestureHook();
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}

HWND createMainWindow(HINSTANCE hInstance) {
  WNDCLASS wc = {};
  wc.hInstance = hInstance;
  wc.lpfnWndProc = WindowProc;
  wc.lpszClassName = L"SquareClass";
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

  if (!RegisterClass(&wc)) {
    MessageBox(nullptr, L"Failed to register window class", L"Error", MB_OK);
    return nullptr;
  }
  HWND hwnd = CreateWindowEx(0, L"SquareClass", L"Square Pro",
                             WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                             500, 500, nullptr, nullptr, hInstance, nullptr);
  if (!hwnd) {
    MessageBox(nullptr, L"Failed to create window", L"Error", MB_OK);
    return nullptr;
  }
  return hwnd;
}
