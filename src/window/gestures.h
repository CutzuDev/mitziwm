#pragma once
#include <windows.h>

enum class SnapAction {
    Left,
    Right,
    Top,
    Bottom,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    Maximize,
    Center,
    Restore,
};

struct GestureConfig {
    SnapAction up          = SnapAction::Top;
    SnapAction down        = SnapAction::Bottom;
    SnapAction left        = SnapAction::Left;
    SnapAction right       = SnapAction::Right;
    SnapAction topLeft     = SnapAction::TopLeft;
    SnapAction topRight    = SnapAction::TopRight;
    SnapAction bottomLeft  = SnapAction::BottomLeft;
    SnapAction bottomRight = SnapAction::BottomRight;
};

extern GestureConfig gestureConfig;

void installGestureHook();
void removeGestureHook();
