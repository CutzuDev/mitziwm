#pragma once
#include <windows.h>


RECT getMonitorWorkArea(HWND hwnd);
HWND getWindowUnderCursor();


// Halves
void snapLeft(HWND hwnd);
void snapRight(HWND hwnd);
void snapCenterHalf(HWND hwnd);
void snapTop(HWND hwnd);
void snapBottom(HWND hwnd);

// Quarters
void snapTopLeft(HWND hwnd);
void snapTopRight(HWND hwnd);
void snapBottomLeft(HWND hwnd);
void snapBottomRight(HWND hwnd);

// Thirds (horizontal)
void snapFirstThird(HWND hwnd);       // left 1/3
void snapCenterThird(HWND hwnd);      // middle 1/3
void snapLastThird(HWND hwnd);        // right 1/3

// Two thirds (horizontal)
void snapFirstTwoThirds(HWND hwnd);   // left 2/3
void snapCenterTwoThirds(HWND hwnd);  // center 2/3
void snapLastTwoThirds(HWND hwnd);    // right 2/3

// Fourths (horizontal)
void snapFirstFourth(HWND hwnd);      // left 1/4
void snapSecondFourth(HWND hwnd);     // second 1/4
void snapThirdFourth(HWND hwnd);      // third 1/4
void snapLastFourth(HWND hwnd);       // right 1/4

// Three fourths (horizontal)
void snapFirstThreeFourths(HWND hwnd); // left 3/4
void snapLastThreeFourths(HWND hwnd);  // right 3/4

// Sixths (top and bottom rows of thirds)
void snapTopFirstThird(HWND hwnd);
void snapTopCenterThird(HWND hwnd);
void snapTopLastThird(HWND hwnd);
void snapBottomFirstThird(HWND hwnd);
void snapBottomCenterThird(HWND hwnd);
void snapBottomLastThird(HWND hwnd);
void snapFirstSixth(HWND hwnd);       // full height, leftmost 1/6
void snapLastSixth(HWND hwnd);        // full height, rightmost 1/6

// Special
void snapMaximize(HWND hwnd);
void snapAlmostMaximize(HWND hwnd);
void snapMaximizeHeight(HWND hwnd);   // full height, keep current width
void snapCenter(HWND hwnd);           // centered, original size
void snapCenterProminently(HWND hwnd); // centered, larger size (like a focused window)
void snapRestore(HWND hwnd);          // restore original size and position

// Multi-monitor
void snapNextMonitor(HWND hwnd);      // throw window to next monitor
void snapPrevMonitor(HWND hwnd);      // throw window to previous monitor