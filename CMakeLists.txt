cmake_minimum_required(VERSION 3.15)
project(Spidercam)

set (CMAKE_CXX_STANDARD 11)

include_directories(.)
add_executable(spidercam inputData.cpp simulation.cpp isimulation.cpp main.cpp
                         inputData.h simulation.h isimulation.h)
