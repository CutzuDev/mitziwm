cmake_minimum_required(VERSION 3.23)
project(MitziWM)

set(CMAKE_CXX_STANDARD 17)

add_executable(MitziWM WIN32
    src/main.cpp
    src/window.cpp
    src/window/snapping.cpp
    src/window/gestures.cpp
    src/window/overlay.cpp
    src/tray/tray.cpp
    src/tray/hotkey_dialog.cpp
    src/config/config.cpp
    ${CMAKE_BINARY_DIR}/resources.rc
)

target_link_libraries(MitziWM
    user32
    shell32
    gdi32
    dwmapi
    advapi32
)

target_compile_definitions(MitziWM PRIVATE
    UNICODE
    _UNICODE
    _AMD64_
    WIN32_LEAN_AND_MEAN
)

configure_file(resources.rc.in ${CMAKE_BINARY_DIR}/resources.rc @ONLY)

install(TARGETS MitziWM RUNTIME DESTINATION .)

set(CPACK_PACKAGE_NAME "MitziWM")
set(CPACK_PACKAGE_VENDOR "CutzuDev")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MitziWM Window Manager")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "MitziWM")
set(CPACK_GENERATOR "NSIS")

set(CPACK_NSIS_DISPLAY_NAME "MitziWM")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
set(CPACK_NSIS_MANIFEST_DPI_AWARE TRUE)
set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/icon.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/icon.ico")
set(CPACK_NSIS_CREATE_ICONS_EXTRA
    "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\MitziWM.lnk' '$INSTDIR\\\\MitziWM.exe'")
set(CPACK_NSIS_DELETE_ICONS_EXTRA
    "Delete '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\MitziWM.lnk'")

include(CPack)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
