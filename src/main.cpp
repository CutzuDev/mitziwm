#include "window.h"
#include "window/gestures.h"
#include "window/overlay.h"
#include "tray/tray.h"
#include <windows.h>

static void enableDarkMode() {
    // SetPreferredAppMode is undocumented, exported by ordinal from uxtheme.dll
    // ForceDark = 2
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

    HANDLE mutex = CreateMutex(nullptr, TRUE, L"my-wm-instance");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        MessageBox(nullptr, L"my-wm is already running.", L"my-wm", MB_OK | MB_ICONINFORMATION);
        return 0;
    }

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
    return 0;
}
