#include "gestures.h"
#include "overlay.h"
#include "snapping.h"
#include <cstdlib>
#include <windows.h>

static HHOOK hookHandle      = nullptr;
static HHOOK mouseHookHandle = nullptr;
static HWND  gestureWindow   = nullptr;
static bool  gestureActive   = false;
static bool  overlayShown    = false;
static bool  ctrlHeld        = false;
static bool  shiftHeld       = false;
static bool  altHeld         = false;
static bool  winHeld         = false;
static bool  xbutton1Held    = false;
static bool  xbutton2Held    = false;
static POINT gestureStart;

static const UINT OVERLAY_DELAY_TIMER = 1002;
static const int  MIN_PIXELS          = 10;

GestureConfig gestureConfig;
HotkeyConfig  hotkeyConfig;

static bool hotkeySatisfied() {
    return (hotkeyConfig.ctrl || hotkeyConfig.shift || hotkeyConfig.alt || hotkeyConfig.win || hotkeyConfig.xbutton1 || hotkeyConfig.xbutton2)
        && (!hotkeyConfig.ctrl     || ctrlHeld)
        && (!hotkeyConfig.shift    || shiftHeld)
        && (!hotkeyConfig.alt      || altHeld)
        && (!hotkeyConfig.win      || winHeld)
        && (!hotkeyConfig.xbutton1 || xbutton1Held)
        && (!hotkeyConfig.xbutton2 || xbutton2Held);
}

static void executeSnap(HWND target, SnapAction action) {
    switch (action) {
        case SnapAction::Maximize:          snapMaximize(target);         break;
        case SnapAction::AlmostMaximize:    snapAlmostMaximize(target);   break;
        case SnapAction::MaximizeHeight:    snapMaximizeHeight(target);   break;
        case SnapAction::Left:              snapLeft(target);             break;
        case SnapAction::Right:             snapRight(target);            break;
        case SnapAction::CenterHalf:        snapCenterHalf(target);       break;
        case SnapAction::Top:               snapTop(target);              break;
        case SnapAction::Bottom:            snapBottom(target);           break;
        case SnapAction::TopLeft:           snapTopLeft(target);          break;
        case SnapAction::TopRight:          snapTopRight(target);         break;
        case SnapAction::BottomLeft:        snapBottomLeft(target);       break;
        case SnapAction::BottomRight:       snapBottomRight(target);      break;
        case SnapAction::FirstThird:        snapFirstThird(target);       break;
        case SnapAction::CenterThird:       snapCenterThird(target);      break;
        case SnapAction::LastThird:         snapLastThird(target);        break;
        case SnapAction::FirstTwoThirds:    snapFirstTwoThirds(target);   break;
        case SnapAction::CenterTwoThirds:   snapCenterTwoThirds(target);  break;
        case SnapAction::LastTwoThirds:     snapLastTwoThirds(target);    break;
        case SnapAction::FirstFourth:       snapFirstFourth(target);      break;
        case SnapAction::SecondFourth:      snapSecondFourth(target);     break;
        case SnapAction::ThirdFourth:       snapThirdFourth(target);      break;
        case SnapAction::LastFourth:        snapLastFourth(target);       break;
        case SnapAction::FirstThreeFourths: snapFirstThreeFourths(target);break;
        case SnapAction::LastThreeFourths:  snapLastThreeFourths(target); break;
        case SnapAction::TopFirstThird:     snapTopFirstThird(target);    break;
        case SnapAction::TopCenterThird:    snapTopCenterThird(target);   break;
        case SnapAction::TopLastThird:      snapTopLastThird(target);     break;
        case SnapAction::BottomFirstThird:  snapBottomFirstThird(target); break;
        case SnapAction::BottomCenterThird: snapBottomCenterThird(target);break;
        case SnapAction::BottomLastThird:   snapBottomLastThird(target);  break;
        case SnapAction::FirstSixth:        snapFirstSixth(target);       break;
        case SnapAction::LastSixth:         snapLastSixth(target);        break;
        case SnapAction::Center:            snapCenter(target);           break;
        case SnapAction::CenterProminently: snapCenterProminently(target);break;
        case SnapAction::Restore:           snapRestore(target);          break;
        default: break;
    }
}

static void handleGesture(POINT start, POINT end) {
    int dx = end.x - start.x;
    int dy = end.y - start.y;

    if (dx * dx + dy * dy < MIN_PIXELS * MIN_PIXELS) return;

    HWND target = getWindowUnderCursor();
    if (!target) return;

    if (abs(dx) > abs(dy)) {
        executeSnap(target, dx > 0 ? gestureConfig.right : gestureConfig.left);
    } else {
        executeSnap(target, dy > 0 ? gestureConfig.down : gestureConfig.up);
    }
    SetWindowPos(target, HWND_TOPMOST,    0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    SetWindowPos(target, HWND_NOTOPMOST,  0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

void onOverlayTimer() {
    KillTimer(gestureWindow, OVERLAY_DELAY_TIMER);
    if (gestureActive) {
        showOverlay(gestureStart);
        overlayShown = true;
    }
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode != HC_ACTION)
        return CallNextHookEx(nullptr, nCode, wParam, lParam);

    KBDLLHOOKSTRUCT* key = (KBDLLHOOKSTRUCT*)lParam;

    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        if (key->vkCode == VK_LCONTROL || key->vkCode == VK_RCONTROL) ctrlHeld  = true;
        if (key->vkCode == VK_LSHIFT   || key->vkCode == VK_RSHIFT)   shiftHeld = true;
        if (key->vkCode == VK_LMENU    || key->vkCode == VK_RMENU)    altHeld   = true;
        if (key->vkCode == VK_LWIN     || key->vkCode == VK_RWIN)     winHeld   = true;

        if (hotkeySatisfied() && !gestureActive) {
            GetCursorPos(&gestureStart);
            gestureActive = true;
            SetTimer(gestureWindow, OVERLAY_DELAY_TIMER, 25, nullptr);
        }
    }

    if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
        if (key->vkCode == VK_LCONTROL || key->vkCode == VK_RCONTROL) ctrlHeld  = false;
        if (key->vkCode == VK_LSHIFT   || key->vkCode == VK_RSHIFT)   shiftHeld = false;
        if (key->vkCode == VK_LMENU    || key->vkCode == VK_RMENU)    altHeld   = false;
        if (key->vkCode == VK_LWIN     || key->vkCode == VK_RWIN)     winHeld   = false;

        if (gestureActive && !hotkeySatisfied()) {
            KillTimer(gestureWindow, OVERLAY_DELAY_TIMER);
            if (overlayShown) {
                hideOverlay();
                overlayShown = false;
            }
            POINT gestureEnd;
            GetCursorPos(&gestureEnd);
            handleGesture(gestureStart, gestureEnd);
            gestureActive = false;
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode != HC_ACTION)
        return CallNextHookEx(nullptr, nCode, wParam, lParam);

    MSLLHOOKSTRUCT* ms = (MSLLHOOKSTRUCT*)lParam;

    if (wParam == WM_XBUTTONDOWN) {
        if (HIWORD(ms->mouseData) == XBUTTON1) xbutton1Held = true;
        if (HIWORD(ms->mouseData) == XBUTTON2) xbutton2Held = true;

        if (hotkeySatisfied() && !gestureActive) {
            GetCursorPos(&gestureStart);
            gestureActive = true;
            SetTimer(gestureWindow, OVERLAY_DELAY_TIMER, 25, nullptr);
        }
    }

    if (wParam == WM_XBUTTONUP) {
        if (HIWORD(ms->mouseData) == XBUTTON1) xbutton1Held = false;
        if (HIWORD(ms->mouseData) == XBUTTON2) xbutton2Held = false;

        if (gestureActive && !hotkeySatisfied()) {
            KillTimer(gestureWindow, OVERLAY_DELAY_TIMER);
            if (overlayShown) {
                hideOverlay();
                overlayShown = false;
            }
            POINT gestureEnd;
            GetCursorPos(&gestureEnd);
            handleGesture(gestureStart, gestureEnd);
            gestureActive = false;
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

void installGestureHook(HWND hwnd) {
    gestureWindow   = hwnd;
    hookHandle      = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, nullptr, 0);
    mouseHookHandle = SetWindowsHookEx(WH_MOUSE_LL,    LowLevelMouseProc,    nullptr, 0);
}

void removeGestureHook() {
    UnhookWindowsHookEx(hookHandle);
    UnhookWindowsHookEx(mouseHookHandle);
}
