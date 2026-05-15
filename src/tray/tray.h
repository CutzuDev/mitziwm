#pragma once
#include <windows.h>

#define WM_TRAY (WM_APP + 1)

void addTrayIcon(HWND hwnd);
void removeTrayIcon();
void showTrayMenu(HWND hwnd);
