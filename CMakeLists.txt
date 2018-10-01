cmake_minimum_required(VERSION 3.8)
project(LoadBalance)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(LoadBalance ${SOURCE_FILES})