#pragma once
#include <windows.h>

enum class SnapAction {
    // Maximize
    Maximize = 0,
    AlmostMaximize,
    MaximizeHeight,

    // Halves
    Left,
    Right,
    CenterHalf,
    Top,
    Bottom,

    // Corners
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,

    // Thirds
    FirstThird,
    CenterThird,
    LastThird,
    FirstTwoThirds,
    CenterTwoThirds,
    LastTwoThirds,

    // Fourths
    FirstFourth,
    SecondFourth,
    ThirdFourth,
    LastFourth,
    FirstThreeFourths,
    LastThreeFourths,

    // Sixths
    TopFirstThird,
    TopCenterThird,
    TopLastThird,
    BottomFirstThird,
    BottomCenterThird,
    BottomLastThird,
    FirstSixth,
    LastSixth,

    // Special
    Center,
    CenterProminently,
    Restore,

    Count
};

struct GestureConfig {
    SnapAction up    = SnapAction::Top;
    SnapAction down  = SnapAction::Bottom;
    SnapAction left  = SnapAction::Left;
    SnapAction right = SnapAction::Right;
};

extern GestureConfig gestureConfig;

void installGestureHook();
void removeGestureHook();
