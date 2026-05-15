#include "gestures.h"
#include "snapping.h"
#include <cstdlib>
#include <windef.h>
#include <windows.h>
#include <winuser.h>

static HHOOK hookHandle = nullptr;
static bool gestureActive = false;
static bool ctrlHeld = false;
static bool shiftHeld = false;
static POINT gestureStart;

void handleGesture(POINT start, POINT end) {
  int dx = end.x - start.x;
  int dy = end.y - start.y;
  HWND target = getWindowUnderCursor();
  if (!target)
    return;
  if (abs(dx) > abs(dy)) {
    if (dx > 0) {
      // snap right
      snapRight(target);
    }
    if (dx < 0) {
      // snap right
      snapLeft(target);
    }
  } else {
    if (dy > 0) {
      // snap bottom
      snapBottom(target);
    }
    if (dy < 0) {
      // snap top
      //
      snapCenter(target);
    }
  }
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode != HC_ACTION) {
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
  }

  KBDLLHOOKSTRUCT *key = (KBDLLHOOKSTRUCT *)lParam;

  if (wParam == WM_KEYDOWN) {
    if (key->vkCode == VK_CONTROL)
      ctrlHeld = true;
    if (key->vkCode == VK_SHIFT)
      shiftHeld = true;
    if (ctrlHeld && shiftHeld && !gestureActive) {
      GetCursorPos(&gestureStart);
      gestureActive = true;
    }
  }

  if (wParam == WM_KEYUP) {
    if (key->vkCode == VK_CONTROL)
      ctrlHeld = false;
    if (key->vkCode == VK_SHIFT)
      shiftHeld = false;
    
    if (gestureActive && (!ctrlHeld || !shiftHeld)) {
      POINT gestureEnd;
      GetCursorPos(&gestureEnd);
      handleGesture(gestureStart, gestureEnd);
      gestureActive = false;
    }
  }
  return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

void installGestureHook() {
  hookHandle =
      SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, nullptr, 0);
}

void removeGestureHook() { UnhookWindowsHookEx(hookHandle); }
