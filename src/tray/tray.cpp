#include "tray.h"
#include "../window/gestures.h"
#include <shellapi.h>

static NOTIFYICONDATA nid = {};

static const int MENU_GESTURE_BASE = 2000;
static const int MENU_QUIT         = 1;

static const wchar_t* snapActionName(SnapAction a) {
    switch (a) {
        case SnapAction::Left:        return L"Snap Left";
        case SnapAction::Right:       return L"Snap Right";
        case SnapAction::Top:         return L"Snap Top";
        case SnapAction::Bottom:      return L"Snap Bottom";
        case SnapAction::TopLeft:     return L"Top Left";
        case SnapAction::TopRight:    return L"Top Right";
        case SnapAction::BottomLeft:  return L"Bottom Left";
        case SnapAction::BottomRight: return L"Bottom Right";
        case SnapAction::Maximize:    return L"Maximize";
        case SnapAction::Center:      return L"Center";
        case SnapAction::Restore:     return L"Restore";
        default:                      return L"Unknown";
    }
}

static SnapAction currentActionForDir(int dirIndex) {
    switch (dirIndex) {
        case 0: return gestureConfig.left;
        case 1: return gestureConfig.right;
        case 2: return gestureConfig.up;
        case 3: return gestureConfig.down;
        default: return SnapAction::Left;
    }
}

static HMENU buildSnapSubmenu(int dirIndex) {
    HMENU sub = CreatePopupMenu();
    SnapAction actions[] = {
        SnapAction::Left, SnapAction::Right, SnapAction::Top, SnapAction::Bottom,
        SnapAction::TopLeft, SnapAction::TopRight, SnapAction::BottomLeft, SnapAction::BottomRight,
        SnapAction::Maximize, SnapAction::Center, SnapAction::Restore
    };
    SnapAction current = currentActionForDir(dirIndex);
    for (SnapAction a : actions) {
        int  id    = MENU_GESTURE_BASE + dirIndex * 100 + (int)a;
        UINT flags = MF_STRING | (a == current ? MF_CHECKED : MF_UNCHECKED);
        AppendMenu(sub, flags, id, snapActionName(a));
    }
    return sub;
}

void addTrayIcon(HWND hwnd) {
    nid.cbSize           = sizeof(NOTIFYICONDATA);
    nid.hWnd             = hwnd;
    nid.uID              = 1;
    nid.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAY;
    nid.hIcon            = LoadIcon(nullptr, IDI_APPLICATION);
    lstrcpy(nid.szTip, L"my-wm");
    Shell_NotifyIcon(NIM_ADD, &nid);
}

void removeTrayIcon() {
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

void showTrayMenu(HWND hwnd) {
    HMENU menu = CreatePopupMenu();

    AppendMenu(menu, MF_POPUP, (UINT_PTR)buildSnapSubmenu(0), L"Left Gesture");
    AppendMenu(menu, MF_POPUP, (UINT_PTR)buildSnapSubmenu(1), L"Right Gesture");
    AppendMenu(menu, MF_POPUP, (UINT_PTR)buildSnapSubmenu(2), L"Up Gesture");
    AppendMenu(menu, MF_POPUP, (UINT_PTR)buildSnapSubmenu(3), L"Down Gesture");
    AppendMenu(menu, MF_SEPARATOR, 0, nullptr);
    AppendMenu(menu, MF_STRING, MENU_QUIT, L"Quit");

    POINT pt;
    GetCursorPos(&pt);

    // Required so the menu dismisses when clicking elsewhere
    SetForegroundWindow(hwnd);
    TrackPopupMenu(menu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, nullptr);
    PostMessage(hwnd, WM_NULL, 0, 0);

    DestroyMenu(menu);
}
