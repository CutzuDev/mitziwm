#pragma once
#include <windows.h>

#define WM_TRAY        (WM_APP + 1)
#define MENU_QUIT      1
#define MENU_STARTUP   2

void addTrayIcon(HWND hwnd);
void removeTrayIcon();
void showTrayMenu(HWND hwnd);
