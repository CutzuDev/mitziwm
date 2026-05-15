#include "window.h"
#include "window/gestures.h"
#include <winbase.h>
#include <minwindef.h>
#include <windef.h>
#include <windows.h>
#include <winuser.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd) {
  HWND hwnd = createMainWindow(hInstance);

  if (!hwnd)
    return 1;

  installGestureHook();
  ShowWindow(hwnd, SW_SHOWNORMAL);
  UpdateWindow(hwnd);

  MSG msg = {};

  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}
