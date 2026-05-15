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
};

struct GestureConfig {
  SnapAction up, down, left, right;
  SnapAction topLeft, topRight, bottomLeft, bottomRight;
};

void installGestureHook();
void removeGestureHook();
