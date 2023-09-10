cmake_minimum_required(VERSION 3.14)

if (WIN32)
    project(JPG-to-ASCII LANGUAGES CXX)
elseif(UNIX)
    project(JPG-to-ASCII)
endif()

include_directories(src)

add_subdirectory(src)
