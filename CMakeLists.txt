cmake_minimum_required(VERSION 3.20)
project(MitziWM)

set(CMAKE_CXX_STANDARD 17)

add_executable(MitziWM WIN32
    src/main.cpp
    src/window.cpp
    src/window/snapping.cpp
    src/window/gestures.cpp
    src/window/overlay.cpp
    src/tray/tray.cpp
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

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
