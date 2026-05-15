#include "window.h"
#include "window/gestures.h"
#include "window/overlay.h"
#include "tray/tray.h"
#include "config/config.h"
#include <windows.h>

static void enableDarkMode() {
    typedef int (WINAPI* fnSetPreferredAppMode)(int);
    HMODULE ux = LoadLibraryW(L"uxtheme.dll");
    if (ux) {
        auto fn = (fnSetPreferredAppMode)GetProcAddress(ux, MAKEINTRESOURCEA(135));
        if (fn) fn(2);
        FreeLibrary(ux);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd) {
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    enableDarkMode();

    HANDLE mutex = CreateMutex(nullptr, TRUE, L"MitziWM-instance");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        MessageBox(nullptr, L"MitziWM is already running.", L"MitziWM", MB_OK | MB_ICONINFORMATION);
        return 0;
    }

    loadConfig();

    HWND hwnd = createMainWindow(hInstance);
    if (!hwnd) return 1;

    installGestureHook();
    createOverlay(hInstance, hwnd);
    addTrayIcon(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CloseHandle(mutex);
    return 0;
}
