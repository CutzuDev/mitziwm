#include "gestures.h"
#include "overlay.h"
#include "snapping.h"
#include <cstdlib>
#include <windows.h>

static HHOOK hookHandle   = nullptr;
static bool  gestureActive = false;
static bool  ctrlHeld     = false;
static bool  shiftHeld    = false;
static POINT gestureStart;

GestureConfig gestureConfig;

static void executeSnap(HWND target, SnapAction action) {
    switch (action) {
        case SnapAction::Left:        snapLeft(target);        break;
        case SnapAction::Right:       snapRight(target);       break;
        case SnapAction::Top:         snapTop(target);         break;
        case SnapAction::Bottom:      snapBottom(target);      break;
        case SnapAction::TopLeft:     snapTopLeft(target);     break;
        case SnapAction::TopRight:    snapTopRight(target);    break;
        case SnapAction::BottomLeft:  snapBottomLeft(target);  break;
        case SnapAction::BottomRight: snapBottomRight(target); break;
        case SnapAction::Maximize:    snapMaximize(target);    break;
        case SnapAction::Center:      snapCenter(target);      break;
        case SnapAction::Restore:     snapRestore(target);     break;
    }
}

static void handleGesture(POINT start, POINT end) {
    int dx = end.x - start.x;
    int dy = end.y - start.y;

    HWND target = getWindowUnderCursor();
    if (!target) return;

    if (abs(dx) > abs(dy)) {
        executeSnap(target, dx > 0 ? gestureConfig.right : gestureConfig.left);
    } else {
        executeSnap(target, dy > 0 ? gestureConfig.down : gestureConfig.up);
    }
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode != HC_ACTION)
        return CallNextHookEx(nullptr, nCode, wParam, lParam);

    KBDLLHOOKSTRUCT* key = (KBDLLHOOKSTRUCT*)lParam;

    if (wParam == WM_KEYDOWN) {
        if (key->vkCode == VK_LCONTROL || key->vkCode == VK_RCONTROL) ctrlHeld = true;
        if (key->vkCode == VK_LSHIFT   || key->vkCode == VK_RSHIFT)   shiftHeld = true;

        if (ctrlHeld && shiftHeld && !gestureActive) {
            GetCursorPos(&gestureStart);
            gestureActive = true;
            showOverlay(gestureStart);
        }
    }

    if (wParam == WM_KEYUP) {
        if (key->vkCode == VK_LCONTROL || key->vkCode == VK_RCONTROL) ctrlHeld = false;
        if (key->vkCode == VK_LSHIFT   || key->vkCode == VK_RSHIFT)   shiftHeld = false;

        if (gestureActive && (!ctrlHeld || !shiftHeld)) {
            POINT gestureEnd;
            GetCursorPos(&gestureEnd);
            hideOverlay();
            handleGesture(gestureStart, gestureEnd);
            gestureActive = false;
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

void installGestureHook() {
    hookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, nullptr, 0);
}

void removeGestureHook() {
    UnhookWindowsHookEx(hookHandle);
}
