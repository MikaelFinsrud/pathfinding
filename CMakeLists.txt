cmake_minimum_required(VERSION 4.3)
project(pathfinding)

set(CMAKE_CXX_STANDARD 20)

add_executable(pathfinding
        main.cpp
        PathfindingAlgorithms.cpp
        Position.h
        PathfindingAlgorithms.h)
