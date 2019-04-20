cmake_minimum_required(VERSION 3.14.0)

project(Learning_OpenGL)

set(CMAKE_CXX_STANDARD)

if(WIN32)
    message("This is a Win32 system")
elseif(!WIN32)
    message("This is a ${CMAKE_SYSTEM_NAME} System")
endif()

add_executable(Learning_OpenGL main.cpp )