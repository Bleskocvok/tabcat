cmake_minimum_required(VERSION 3.18)

project(tabcat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

if(NOT MSVC)
    # settings for the normal compilers
    add_compile_options(-Wall -Wextra -pedantic -O2)
else()
    # settings for MSVC
    add_compile_options(/W4 /Wall)
endif()

add_executable(${PROJECT_NAME} src/tabcat.cpp)
