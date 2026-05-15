cmake_minimum_required(VERSION 3.20)
project(my-wm)

set(CMAKE_CXX_STANDARD 17)

add_executable(my-wm WIN32
    src/main.cpp
    src/window.cpp
    src/window/snapping.cpp
    src/window/gestures.cpp
    src/tray/tray.cpp
)

target_link_libraries(my-wm
    user32
    shell32
    gdi32
)
target_compile_definitions(my-wm PRIVATE
    UNICODE
    _UNICODE
    _AMD64_
    WIN32_LEAN_AND_MEAN
)

# This lets clangd (Zed's LSP) understand your project
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)