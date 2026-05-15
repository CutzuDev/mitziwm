#include "snapping.h"
#include <windef.h>
#include <winuser.h>

RECT getMonitorWorkArea(HWND hwnd) {
  HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
  MONITORINFO info = {};
  info.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(monitor, &info);

  return info.rcWork;
}

HWND getWindowUnderCursor() {
  POINT cursor;
  GetCursorPos(&cursor);
  HWND target = WindowFromPoint(cursor);
  if (!target)
    return nullptr;

  target = GetAncestor(target, GA_ROOTOWNER);
  return target;
}

void snapLeft(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);

  int x = area.left;
  int y = area.top;
  int width = (area.right - area.left) / 2;
  int height = area.bottom - area.top;

  SetWindowPos(hwnd, nullptr, x, y, width, height,
               SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapRight(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);

  int width = (area.right - area.left) / 2;
  int x = area.left + width;
  int y = area.top;
  int height = area.bottom - area.top;

  SetWindowPos(hwnd, nullptr, x, y, width, height,
               SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapCenter(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);

  int width = (area.right - area.left) / 2;
  int x = area.left + (width / 2);
  int y = area.top;
  int height = area.bottom - area.top;
  SetWindowPos(hwnd, nullptr, x, y, width, height,
               SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapBottom(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);

  int width = (area.right - area.left);
  int height = (area.bottom - area.top / 2);
  int x = area.left;
  int y = area.top + (height / 2);
  SetWindowPos(hwnd, nullptr, x, y, width, height,
               SWP_NOZORDER | SWP_NOACTIVATE);
}
