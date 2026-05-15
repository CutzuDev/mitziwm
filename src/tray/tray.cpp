#include "tray.h"
#include "../window/gestures.h"
#include "../config/config.h"
#include <shellapi.h>

static NOTIFYICONDATA nid = {};

static const int MENU_GESTURE_BASE = 2000;

static const wchar_t* snapActionName(SnapAction a) {
    switch (a) {
        case SnapAction::Maximize:          return L"Maximize";
        case SnapAction::AlmostMaximize:    return L"Almost Maximize";
        case SnapAction::MaximizeHeight:    return L"Maximize Height";
        case SnapAction::Left:              return L"Left Half";
        case SnapAction::Right:             return L"Right Half";
        case SnapAction::CenterHalf:        return L"Center Half";
        case SnapAction::Top:               return L"Top Half";
        case SnapAction::Bottom:            return L"Bottom Half";
        case SnapAction::TopLeft:           return L"Top Left";
        case SnapAction::TopRight:          return L"Top Right";
        case SnapAction::BottomLeft:        return L"Bottom Left";
        case SnapAction::BottomRight:       return L"Bottom Right";
        case SnapAction::FirstThird:        return L"First Third";
        case SnapAction::CenterThird:       return L"Center Third";
        case SnapAction::LastThird:         return L"Last Third";
        case SnapAction::FirstTwoThirds:    return L"First Two Thirds";
        case SnapAction::CenterTwoThirds:   return L"Center Two Thirds";
        case SnapAction::LastTwoThirds:     return L"Last Two Thirds";
        case SnapAction::FirstFourth:       return L"First Fourth";
        case SnapAction::SecondFourth:      return L"Second Fourth";
        case SnapAction::ThirdFourth:       return L"Third Fourth";
        case SnapAction::LastFourth:        return L"Last Fourth";
        case SnapAction::FirstThreeFourths: return L"First Three Fourths";
        case SnapAction::LastThreeFourths:  return L"Last Three Fourths";
        case SnapAction::TopFirstThird:     return L"Top Left Sixth";
        case SnapAction::TopCenterThird:    return L"Top Center Sixth";
        case SnapAction::TopLastThird:      return L"Top Right Sixth";
        case SnapAction::BottomFirstThird:  return L"Bottom Left Sixth";
        case SnapAction::BottomCenterThird: return L"Bottom Center Sixth";
        case SnapAction::BottomLastThird:   return L"Bottom Right Sixth";
        case SnapAction::FirstSixth:        return L"First Sixth";
        case SnapAction::LastSixth:         return L"Last Sixth";
        case SnapAction::Center:            return L"Center";
        case SnapAction::CenterProminently: return L"Center Prominently";
        case SnapAction::Restore:           return L"Restore";
        default:                            return L"Unknown";
    }
}

static void addHeader(HMENU menu, const wchar_t* label) {
    AppendMenu(menu, MF_STRING | MF_GRAYED | MF_DISABLED, 0, label);
}

static void addAction(HMENU menu, int dirIndex, SnapAction a, SnapAction current) {
    int  id    = MENU_GESTURE_BASE + dirIndex * 100 + (int)a;
    UINT flags = MF_STRING | (a == current ? MF_CHECKED : MF_UNCHECKED);
    AppendMenu(menu, flags, id, snapActionName(a));
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
    HMENU        sub     = CreatePopupMenu();
    SnapAction   current = currentActionForDir(dirIndex);
    auto         add     = [&](SnapAction a) { addAction(sub, dirIndex, a, current); };

    addHeader(sub, L"Maximize");
    add(SnapAction::Maximize);
    add(SnapAction::AlmostMaximize);
    add(SnapAction::MaximizeHeight);

    AppendMenu(sub, MF_SEPARATOR, 0, nullptr);
    addHeader(sub, L"Halves");
    add(SnapAction::Left);
    add(SnapAction::Right);
    add(SnapAction::CenterHalf);
    add(SnapAction::Top);
    add(SnapAction::Bottom);

    AppendMenu(sub, MF_SEPARATOR, 0, nullptr);
    addHeader(sub, L"Corners");
    add(SnapAction::TopLeft);
    add(SnapAction::TopRight);
    add(SnapAction::BottomLeft);
    add(SnapAction::BottomRight);

    AppendMenu(sub, MF_SEPARATOR, 0, nullptr);
    addHeader(sub, L"Thirds");
    add(SnapAction::FirstThird);
    add(SnapAction::CenterThird);
    add(SnapAction::LastThird);
    add(SnapAction::FirstTwoThirds);
    add(SnapAction::CenterTwoThirds);
    add(SnapAction::LastTwoThirds);

    AppendMenu(sub, MF_SEPARATOR, 0, nullptr);
    addHeader(sub, L"Fourths");
    add(SnapAction::FirstFourth);
    add(SnapAction::SecondFourth);
    add(SnapAction::ThirdFourth);
    add(SnapAction::LastFourth);
    add(SnapAction::FirstThreeFourths);
    add(SnapAction::LastThreeFourths);

    AppendMenu(sub, MF_SEPARATOR, 0, nullptr);
    addHeader(sub, L"Sixths");
    add(SnapAction::TopFirstThird);
    add(SnapAction::TopCenterThird);
    add(SnapAction::TopLastThird);
    add(SnapAction::BottomFirstThird);
    add(SnapAction::BottomCenterThird);
    add(SnapAction::BottomLastThird);
    add(SnapAction::FirstSixth);
    add(SnapAction::LastSixth);

    AppendMenu(sub, MF_SEPARATOR, 0, nullptr);
    addHeader(sub, L"Special");
    add(SnapAction::Center);
    add(SnapAction::CenterProminently);
    add(SnapAction::Restore);

    return sub;
}

void addTrayIcon(HWND hwnd) {
    nid.cbSize           = sizeof(NOTIFYICONDATA);
    nid.hWnd             = hwnd;
    nid.uID              = 1;
    nid.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAY;
    nid.hIcon = (HICON)LoadImage(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_MYWM_ICON), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
    lstrcpy(nid.szTip, L"MitziWM");
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
    UINT startupFlags = MF_STRING | (isStartOnBoot() ? MF_CHECKED : MF_UNCHECKED);
    AppendMenu(menu, startupFlags, MENU_STARTUP, L"Start on Boot");
    AppendMenu(menu, MF_SEPARATOR, 0, nullptr);
    AppendMenu(menu, MF_STRING, MENU_QUIT, L"Quit");

    POINT pt;
    GetCursorPos(&pt);

    SetForegroundWindow(hwnd);
    TrackPopupMenu(menu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, nullptr);
    PostMessage(hwnd, WM_NULL, 0, 0);

    DestroyMenu(menu);
}
