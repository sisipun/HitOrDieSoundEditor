cmake_minimum_required(VERSION 3.16)

project(HitOrDieSoundEditor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "~/Qt/6.3.1/macos/lib/cmake")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

add_executable(HitOrDieSoundEditor 
    sound_editor.ui
    sound_editor.cpp
    main.cpp
)

target_link_libraries(HitOrDieSoundEditor PRIVATE Qt6::Widgets)