#include "config.h"
#include "../window/gestures.h"
#include <cstdio>
#include <shlobj.h>
#include <windows.h>

static wchar_t configPath[MAX_PATH];

static void ensureConfigDir() {
    wchar_t appdata[MAX_PATH];
    SHGetFolderPathW(nullptr, CSIDL_APPDATA, nullptr, 0, appdata);

    wchar_t dir[MAX_PATH];
    swprintf(dir, MAX_PATH, L"%s\\MitziWM", appdata);
    CreateDirectoryW(dir, nullptr); // no-op if already exists

    swprintf(configPath, MAX_PATH, L"%s\\config.ini", dir);
}

static void writeInt(const wchar_t* section, const wchar_t* key, int value) {
    wchar_t buf[16];
    swprintf(buf, 16, L"%d", value);
    WritePrivateProfileStringW(section, key, buf, configPath);
}

void loadConfig() {
    ensureConfigDir();
    gestureConfig.left  = (SnapAction)GetPrivateProfileIntW(L"Gestures", L"Left",  (int)SnapAction::Left,   configPath);
    gestureConfig.right = (SnapAction)GetPrivateProfileIntW(L"Gestures", L"Right", (int)SnapAction::Right,  configPath);
    gestureConfig.up    = (SnapAction)GetPrivateProfileIntW(L"Gestures", L"Up",    (int)SnapAction::Top,    configPath);
    gestureConfig.down  = (SnapAction)GetPrivateProfileIntW(L"Gestures", L"Down",  (int)SnapAction::Bottom, configPath);

    hotkeyConfig.ctrl     = GetPrivateProfileIntW(L"Hotkey", L"Ctrl",     1, configPath) != 0;
    hotkeyConfig.shift    = GetPrivateProfileIntW(L"Hotkey", L"Shift",    1, configPath) != 0;
    hotkeyConfig.alt      = GetPrivateProfileIntW(L"Hotkey", L"Alt",      0, configPath) != 0;
    hotkeyConfig.win      = GetPrivateProfileIntW(L"Hotkey", L"Win",      0, configPath) != 0;
    hotkeyConfig.xbutton1 = GetPrivateProfileIntW(L"Hotkey", L"XButton1", 0, configPath) != 0;
    hotkeyConfig.xbutton2 = GetPrivateProfileIntW(L"Hotkey", L"XButton2", 0, configPath) != 0;
}

void saveConfig() {
    ensureConfigDir();
    writeInt(L"Gestures", L"Left",  (int)gestureConfig.left);
    writeInt(L"Gestures", L"Right", (int)gestureConfig.right);
    writeInt(L"Gestures", L"Up",    (int)gestureConfig.up);
    writeInt(L"Gestures", L"Down",  (int)gestureConfig.down);

    writeInt(L"Hotkey", L"Ctrl",     hotkeyConfig.ctrl     ? 1 : 0);
    writeInt(L"Hotkey", L"Shift",   hotkeyConfig.shift    ? 1 : 0);
    writeInt(L"Hotkey", L"Alt",     hotkeyConfig.alt      ? 1 : 0);
    writeInt(L"Hotkey", L"Win",     hotkeyConfig.win      ? 1 : 0);
    writeInt(L"Hotkey", L"XButton1",hotkeyConfig.xbutton1 ? 1 : 0);
    writeInt(L"Hotkey", L"XButton2",hotkeyConfig.xbutton2 ? 1 : 0);
}

bool isStartOnBoot() {
    HKEY key;
    if (RegOpenKeyExW(HKEY_CURRENT_USER,
                      L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                      0, KEY_READ, &key) != ERROR_SUCCESS)
        return false;
    bool exists = RegQueryValueExW(key, L"MitziWM", nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS;
    RegCloseKey(key);
    return exists;
}

void setStartOnBoot(bool enabled) {
    HKEY key;
    RegOpenKeyExW(HKEY_CURRENT_USER,
                  L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                  0, KEY_WRITE, &key);
    if (enabled) {
        wchar_t path[MAX_PATH];
        GetModuleFileNameW(nullptr, path, MAX_PATH);
        RegSetValueExW(key, L"MitziWM", 0, REG_SZ,
                       (BYTE*)path, (DWORD)((wcslen(path) + 1) * sizeof(wchar_t)));
    } else {
        RegDeleteValueW(key, L"MitziWM");
    }
    RegCloseKey(key);
}
