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

// Halves

void snapLeft(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int x = area.left;
  int y = area.top;
  int width = (area.right - area.left) / 2;
  int height = area.bottom - area.top;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapRight(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int width = (area.right - area.left) / 2;
  int x = area.left + width;
  int y = area.top;
  int height = area.bottom - area.top;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapTop(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int x = area.left;
  int y = area.top;
  int width = area.right - area.left;
  int height = (area.bottom - area.top) / 2;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapBottom(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int width = area.right - area.left;
  int height = (area.bottom - area.top) / 2;
  int x = area.left;
  int y = area.top + height;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

// Quarters

void snapTopLeft(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int x = area.left;
  int y = area.top;
  int width = (area.right - area.left) / 2;
  int height = (area.bottom - area.top) / 2;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapTopRight(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int width = (area.right - area.left) / 2;
  int x = area.left + width;
  int y = area.top;
  int height = (area.bottom - area.top) / 2;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapBottomLeft(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int x = area.left;
  int width = (area.right - area.left) / 2;
  int height = (area.bottom - area.top) / 2;
  int y = area.top + height;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapBottomRight(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int width = (area.right - area.left) / 2;
  int height = (area.bottom - area.top) / 2;
  int x = area.left + width;
  int y = area.top + height;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

// Thirds

void snapFirstThird(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int x = area.left;
  int y = area.top;
  int width = (area.right - area.left) / 3;
  int height = area.bottom - area.top;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapCenterThird(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int width = (area.right - area.left) / 3;
  int x = area.left + width;
  int y = area.top;
  int height = area.bottom - area.top;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapLastThird(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int width = (area.right - area.left) / 3;
  int x = area.right - width;
  int y = area.top;
  int height = area.bottom - area.top;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

// Two thirds

void snapFirstTwoThirds(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int x = area.left;
  int y = area.top;
  int width = (area.right - area.left) * 2 / 3;
  int height = area.bottom - area.top;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapLastTwoThirds(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int width = (area.right - area.left) * 2 / 3;
  int x = area.right - width;
  int y = area.top;
  int height = area.bottom - area.top;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

// Fourths

void snapFirstFourth(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int x = area.left;
  int y = area.top;
  int width = (area.right - area.left) / 4;
  int height = area.bottom - area.top;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapSecondFourth(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int width = (area.right - area.left) / 4;
  int x = area.left + width;
  int y = area.top;
  int height = area.bottom - area.top;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapThirdFourth(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int width = (area.right - area.left) / 4;
  int x = area.left + width * 2;
  int y = area.top;
  int height = area.bottom - area.top;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapLastFourth(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int width = (area.right - area.left) / 4;
  int x = area.right - width;
  int y = area.top;
  int height = area.bottom - area.top;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

// Three fourths

void snapFirstThreeFourths(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int x = area.left;
  int y = area.top;
  int width = (area.right - area.left) * 3 / 4;
  int height = area.bottom - area.top;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapLastThreeFourths(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int width = (area.right - area.left) * 3 / 4;
  int x = area.right - width;
  int y = area.top;
  int height = area.bottom - area.top;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

// Sixths

void snapTopFirstThird(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int x = area.left;
  int y = area.top;
  int width = (area.right - area.left) / 3;
  int height = (area.bottom - area.top) / 2;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapTopCenterThird(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int width = (area.right - area.left) / 3;
  int x = area.left + width;
  int y = area.top;
  int height = (area.bottom - area.top) / 2;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapTopLastThird(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int width = (area.right - area.left) / 3;
  int x = area.right - width;
  int y = area.top;
  int height = (area.bottom - area.top) / 2;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapBottomFirstThird(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int x = area.left;
  int width = (area.right - area.left) / 3;
  int height = (area.bottom - area.top) / 2;
  int y = area.top + height;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapBottomCenterThird(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int width = (area.right - area.left) / 3;
  int x = area.left + width;
  int height = (area.bottom - area.top) / 2;
  int y = area.top + height;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapBottomLastThird(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int width = (area.right - area.left) / 3;
  int x = area.right - width;
  int height = (area.bottom - area.top) / 2;
  int y = area.top + height;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

// Special

void snapMaximize(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int x = area.left;
  int y = area.top;
  int width = area.right - area.left;
  int height = area.bottom - area.top;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapAlmostMaximize(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int margin = 8;
  int x = area.left + margin;
  int y = area.top + margin;
  int width = (area.right - area.left) - margin * 2;
  int height = (area.bottom - area.top) - margin * 2;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapCenter(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  RECT wndRect;
  GetWindowRect(hwnd, &wndRect);
  int wndW = wndRect.right - wndRect.left;
  int wndH = wndRect.bottom - wndRect.top;
  int areaW = area.right - area.left;
  int areaH = area.bottom - area.top;
  int x = area.left + (areaW - wndW) / 2;
  int y = area.top + (areaH - wndH) / 2;
  SetWindowPos(hwnd, nullptr, x, y, wndW, wndH, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapCenterProminently(HWND hwnd) {
  RECT area = getMonitorWorkArea(hwnd);
  int areaW = area.right - area.left;
  int areaH = area.bottom - area.top;
  int width = areaW * 3 / 4;
  int height = areaH * 3 / 4;
  int x = area.left + (areaW - width) / 2;
  int y = area.top + (areaH - height) / 2;
  SetWindowPos(hwnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapRestore(HWND hwnd) {
  ShowWindow(hwnd, SW_RESTORE);
}

// Multi-monitor

static BOOL CALLBACK collectMonitors(HMONITOR hMon, HDC, LPRECT, LPARAM lParam) {
  HMONITOR *arr = (HMONITOR *)lParam;
  int i = 0;
  while (arr[i]) i++;
  if (i < 16) arr[i] = hMon;
  return TRUE;
}

static void snapToAdjacentMonitor(HWND hwnd, int direction) {
  HMONITOR monitors[16] = {};
  EnumDisplayMonitors(nullptr, nullptr, collectMonitors, (LPARAM)monitors);

  int count = 0;
  while (monitors[count]) count++;
  if (count < 2) return;

  HMONITOR current = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
  int currentIdx = 0;
  for (int i = 0; i < count; i++) {
    if (monitors[i] == current) { currentIdx = i; break; }
  }

  int nextIdx = (currentIdx + direction + count) % count;

  MONITORINFO srcInfo = {}, dstInfo = {};
  srcInfo.cbSize = dstInfo.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(current, &srcInfo);
  GetMonitorInfo(monitors[nextIdx], &dstInfo);

  RECT wndRect;
  GetWindowRect(hwnd, &wndRect);

  int srcW = srcInfo.rcWork.right - srcInfo.rcWork.left;
  int srcH = srcInfo.rcWork.bottom - srcInfo.rcWork.top;
  int dstW = dstInfo.rcWork.right - dstInfo.rcWork.left;
  int dstH = dstInfo.rcWork.bottom - dstInfo.rcWork.top;
  int wndW = wndRect.right - wndRect.left;
  int wndH = wndRect.bottom - wndRect.top;

  float relX = (float)(wndRect.left - srcInfo.rcWork.left) / srcW;
  float relY = (float)(wndRect.top - srcInfo.rcWork.top) / srcH;

  int newX = dstInfo.rcWork.left + (int)(relX * dstW);
  int newY = dstInfo.rcWork.top + (int)(relY * dstH);

  SetWindowPos(hwnd, nullptr, newX, newY, wndW, wndH, SWP_NOZORDER | SWP_NOACTIVATE);
}

void snapNextMonitor(HWND hwnd) { snapToAdjacentMonitor(hwnd, 1); }
void snapPrevMonitor(HWND hwnd) { snapToAdjacentMonitor(hwnd, -1); }
