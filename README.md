# MitziWM

A lightweight window manager for Windows. Hold a modifier combo, drag your mouse in a direction, and the focused window snaps to a position. No taskbar clutter, no config files to hand-edit — just a tray icon and a right-click menu.

Named after my cat.

---

## How it works

Hold your gesture keys (default: **Ctrl + Shift**), then drag the mouse from anywhere on screen:

- **Left / Right** — snap to left or right half
- **Up** — snap to top half
- **Down** — snap to bottom half

A small dot appears under the cursor after a short delay to confirm the gesture is active. Release the keys once you've dragged far enough and the window snaps.

Every direction is independently configurable from the tray menu — you can assign any of ~35 snap positions to each direction (halves, thirds, fourths, sixths, corners, maximize, center, etc.).

The gesture trigger keys are also configurable: right-click the tray → **Change Gesture Key...**, hold whatever modifier combo you want (up to 2 keys from Ctrl / Shift / Alt / Win), then click OK.

Settings are saved to `%APPDATA%\MitziWM\config.ini`.

---

## Building from source

### What you need

- **Visual Studio 2022** (Community is fine) with the **Desktop development with C++** workload  
  Installer: https://visualstudio.microsoft.com/downloads/  
  During install, make sure "MSVC v143" and "Windows 11 SDK" are checked.

- **CMake 3.20+**  
  Either install it standalone from https://cmake.org/download/ or tick **CMake tools for Windows** inside the Visual Studio installer.

- **Git**  
  https://git-scm.com/download/win

That's it. No external libraries — everything else is Win32.

### Build

```bat
git clone https://github.com/CutzuDev/my-wm.git
cd my-wm
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

The binary ends up at `build\Release\MitziWM.exe`. You can run it directly from there.

### Building the installer

You'll also need **NSIS**:  
https://nsis.sourceforge.io/Download

Then after building:

```bat
cd build
cmake ..
cmake --build . --config Release
cpack -C Release
```

This produces `MitziWM-1.0.0-win64.exe` in the build directory — a standard Windows installer that puts the exe in `Program Files\MitziWM\`, adds a Start Menu shortcut, and registers an uninstaller in Add/Remove Programs.

---

## Tray menu

Right-click the tray icon to:

- Set what each gesture direction does
- Toggle **Start on Boot**
- Change the gesture trigger keys
- Quit

---

## Notes

- Only one instance runs at a time — launching a second copy shows a message and exits.
- The Win key works as a modifier but will also open the Start menu — probably best to avoid it.
- Config survives updates since it lives in AppData, not next to the exe.
