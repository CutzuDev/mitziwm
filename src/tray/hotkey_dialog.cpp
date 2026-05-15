#include "hotkey_dialog.h"
#include "../window/gestures.h"
#include "../config/config.h"
#include <string>

static const int ID_LABEL_INST  = 100;
static const int ID_LABEL_COMBO = 101;
static const int ID_BTN_OK      = 102;
static const int ID_BTN_CANCEL  = 103;
static const UINT DLG_TIMER_ID  = 3001;

static const int DLG_CLIENT_W = 300;
static const int DLG_CLIENT_H = 130;

static bool        g_dlgDone   = false;
static bool        g_confirmed = false;
static bool        g_open      = false;
static HotkeyConfig g_captured;

static std::wstring comboString(const HotkeyConfig& h) {
    std::wstring s;
    auto append = [&](const wchar_t* name) {
        if (!s.empty()) s += L" + ";
        s += name;
    };
    if (h.ctrl)     append(L"Ctrl");
    if (h.shift)    append(L"Shift");
    if (h.alt)      append(L"Alt");
    if (h.win)      append(L"Win");
    if (h.xbutton1) append(L"Mouse 4");
    if (h.xbutton2) append(L"Mouse 5");
    return s.empty() ? L"(none)" : s;
}

static void updateLabel(HWND hwnd) {
    std::wstring text = L"Selected: " + comboString(g_captured);
    SetWindowTextW(GetDlgItem(hwnd, ID_LABEL_COMBO), text.c_str());
}

static LRESULT CALLBACK HotkeyDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindowExW(0, L"STATIC",
            L"Hold 1 or 2 modifier keys, then click OK.",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            10, 15, DLG_CLIENT_W - 20, 20,
            hwnd, (HMENU)ID_LABEL_INST, nullptr, nullptr);

        CreateWindowExW(0, L"STATIC", L"",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            10, 45, DLG_CLIENT_W - 20, 25,
            hwnd, (HMENU)ID_LABEL_COMBO, nullptr, nullptr);

        CreateWindowExW(0, L"BUTTON", L"OK",
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            55, 85, 80, 28,
            hwnd, (HMENU)ID_BTN_OK, nullptr, nullptr);

        CreateWindowExW(0, L"BUTTON", L"Cancel",
            WS_CHILD | WS_VISIBLE,
            165, 85, 80, 28,
            hwnd, (HMENU)ID_BTN_CANCEL, nullptr, nullptr);

        g_captured = hotkeyConfig;
        updateLabel(hwnd);
        SetTimer(hwnd, DLG_TIMER_ID, 50, nullptr);
        return 0;
    }

    case WM_TIMER: {
        bool ctrl  = (GetAsyncKeyState(VK_CONTROL)  & 0x8000) != 0;
        bool shift = (GetAsyncKeyState(VK_SHIFT)    & 0x8000) != 0;
        bool alt   = (GetAsyncKeyState(VK_MENU)     & 0x8000) != 0;
        bool win   = ((GetAsyncKeyState(VK_LWIN) | GetAsyncKeyState(VK_RWIN)) & 0x8000) != 0;
        bool xb1   = (GetAsyncKeyState(VK_XBUTTON1) & 0x8000) != 0;
        bool xb2   = (GetAsyncKeyState(VK_XBUTTON2) & 0x8000) != 0;

        int count = (int)ctrl + (int)shift + (int)alt + (int)win + (int)xb1 + (int)xb2;
        if (count >= 1 && count <= 2) {
            g_captured = { ctrl, shift, alt, win, xb1, xb2 };
            updateLabel(hwnd);
        }
        return 0;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BTN_OK) {
            if (!g_captured.ctrl && !g_captured.shift && !g_captured.alt && !g_captured.win && !g_captured.xbutton1 && !g_captured.xbutton2) {
                MessageBoxW(hwnd, L"Please hold at least one modifier key.", L"MitziWM", MB_OK | MB_ICONWARNING);
                return 0;
            }
            g_confirmed = true;
            DestroyWindow(hwnd);
        } else if (LOWORD(wParam) == ID_BTN_CANCEL) {
            DestroyWindow(hwnd);
        }
        return 0;

    case WM_DESTROY:
        KillTimer(hwnd, DLG_TIMER_ID);
        g_dlgDone = true;
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void showHotkeyDialog(HWND parent) {
    if (g_open) return;
    g_open = true;

    HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE);

    static bool registered = false;
    if (!registered) {
        WNDCLASSW wc     = {};
        wc.lpfnWndProc   = HotkeyDlgProc;
        wc.hInstance     = hInst;
        wc.lpszClassName = L"MitziHotkeyDlg";
        wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
        wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        RegisterClassW(&wc);
        registered = true;
    }

    g_dlgDone   = false;
    g_confirmed = false;

    RECT rc = { 0, 0, DLG_CLIENT_W, DLG_CLIENT_H };
    DWORD style   = WS_POPUP | WS_CAPTION | WS_SYSMENU;
    DWORD exStyle = WS_EX_DLGMODALFRAME | WS_EX_TOPMOST;
    AdjustWindowRectEx(&rc, style, FALSE, exStyle);
    int w = rc.right  - rc.left;
    int h = rc.bottom - rc.top;
    int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

    HWND dlg = CreateWindowExW(exStyle, L"MitziHotkeyDlg", L"Set Gesture Key",
        style, x, y, w, h, parent, nullptr, hInst, nullptr);

    ShowWindow(dlg, SW_SHOW);
    UpdateWindow(dlg);

    MSG msg;
    while (!g_dlgDone && GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (g_confirmed) {
        hotkeyConfig = g_captured;
        saveConfig();
    }

    g_open = false;
}
