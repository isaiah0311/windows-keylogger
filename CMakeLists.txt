cmake_minimum_required(VERSION 3.10)

project(windows-keylogger LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} src/main.c)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
        /WX
        /permissive-
        /Zc:preprocessor
        /utf-8
        /sdl
        /MP
    )

    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )
endif()
