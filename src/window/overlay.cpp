#include "overlay.h"

static HWND overlayHwnd = nullptr;
static const int OVERLAY_SIZE = 20;

static void drawAt(POINT pos) {
    if (!overlayHwnd) return;

    HDC screenDC = GetDC(nullptr);
    HDC memDC    = CreateCompatibleDC(screenDC);

    BITMAPINFO bmi              = {};
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = OVERLAY_SIZE;
    bmi.bmiHeader.biHeight      = -OVERLAY_SIZE;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    DWORD*  bits   = nullptr;
    HBITMAP bitmap = CreateDIBSection(memDC, &bmi, DIB_RGB_COLORS, (void**)&bits, nullptr, 0);
    HBITMAP old    = (HBITMAP)SelectObject(memDC, bitmap);

    int  cx    = OVERLAY_SIZE / 2;
    int  cy    = OVERLAY_SIZE / 2;
    int  r     = OVERLAY_SIZE / 2 - 1;
    BYTE alpha = 77; // 30% of 255

    for (int y = 0; y < OVERLAY_SIZE; y++) {
        for (int x = 0; x < OVERLAY_SIZE; x++) {
            int dx = x - cx, dy = y - cy;
            bits[y * OVERLAY_SIZE + x] = (dx * dx + dy * dy <= r * r)
                ? ((DWORD)alpha << 24) | ((DWORD)alpha << 16)
                : 0;
        }
    }

    POINT ptDst = { pos.x - cx, pos.y - cy };
    SIZE  szWnd = { OVERLAY_SIZE, OVERLAY_SIZE };
    POINT ptSrc = { 0, 0 };

    BLENDFUNCTION blend       = {};
    blend.BlendOp             = AC_SRC_OVER;
    blend.SourceConstantAlpha = 255;
    blend.AlphaFormat         = AC_SRC_ALPHA;

    UpdateLayeredWindow(overlayHwnd, screenDC, &ptDst, &szWnd, memDC, &ptSrc, 0, &blend, ULW_ALPHA);

    SelectObject(memDC, old);
    DeleteObject(bitmap);
    DeleteDC(memDC);
    ReleaseDC(nullptr, screenDC);
}

static LRESULT CALLBACK OverlayProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void createOverlay(HINSTANCE hInstance, HWND mainHwnd) {
    WNDCLASS wc      = {};
    wc.lpfnWndProc   = OverlayProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = L"OverlayClass";
    RegisterClass(&wc);

    overlayHwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_NOACTIVATE,
        L"OverlayClass", nullptr, WS_POPUP,
        0, 0, OVERLAY_SIZE, OVERLAY_SIZE,
        nullptr, nullptr, hInstance, nullptr
    );
}

void showOverlay(POINT pos) {
    if (!overlayHwnd) return;
    drawAt(pos);
    ShowWindow(overlayHwnd, SW_SHOWNOACTIVATE);
}

void hideOverlay() {
    if (!overlayHwnd) return;
    ShowWindow(overlayHwnd, SW_HIDE);
}
