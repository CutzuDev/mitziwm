#pragma once
#include <windows.h>

enum class HotkeyAction {
    SnapLeft,
    SnapRight,
    SnapTopLeft,
    SnapTopRight,
    SnapBottomLeft,
    SnapBottomRight,
    SnapMaximize,
};

void installGestureHook();
void removeGestureHook();
