cmake_minimum_required(VERSION 3.16)
project(CPUMonitor)

set(CMAKE_CXX_STANDARD 14)

add_executable(CPUMonitor main.cpp cpumonitor.cpp cpumonitor.h)
