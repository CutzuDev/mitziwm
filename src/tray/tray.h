#pragma once
#include <windows.h>

#define WM_TRAY        (WM_APP + 1)
#define MENU_QUIT      1
#define MENU_STARTUP   2
#define MENU_HOTKEY    3
#include "../../resource.h"

void addTrayIcon(HWND hwnd);
void removeTrayIcon();
void showTrayMenu(HWND hwnd);
