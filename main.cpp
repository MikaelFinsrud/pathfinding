//
// Created by mikke on 8/20/26.
//
#include <iostream>
#include <ostream>
#include <vector>
#include <string>
#include <queue>
#include <array>
#include <algorithm>
#include <random>
#include "Position.h"
#include "PathfindingAlgorithms.h"

const Maze baseMaze {
    "+--------------------+",
    "|....|...............|",
    "|.---|----.----|-....|",
    "|........|...........|",
    "|.---|...|----.|.....|",
    "|....|...............|",
    "+--------------------+",
};

void printMaze(const Maze& maze) {
    for (const std::string& s : maze) {
        for (const char& c : s) {
            std::string color{};
            std::string colorEnd = "\033[0m";

            if (c == '/') {
                color = "\033[32m";
            }
            else if (c == 'X') {
                color = "\033[33m" ;
            }
            else if (c == '|' || c == '-' || c == '+') {
                color = "\033[31m" ;
            }
            std::cout << color << c << colorEnd;
        }
        std::cout << '\n';
    }
}

void drawPath(Maze& maze, const Position& start, const Position& end, const std::optional<Path>& path) {
    if (!path.has_value()) {
        maze[start.y][start.x] = 'X';
        maze[end.y][end.x] = 'X';
        return;
    }

    for (const Position pos : path.value()) {
        maze[pos.y][pos.x] = '/';
    }

    maze[start.y][start.x] = 'X';
    maze[end.y][end.x] = 'X';
}

Position getValidPosition(std::uniform_int_distribution<int> xDistrib, std::uniform_int_distribution<int> yDistrib, std::mt19937& gen, const Maze& maze) {
    Position pos;

    do {
        pos.x = xDistrib(gen);
        pos.y = yDistrib(gen);
    } while (maze[pos.y][pos.x] != '.');

    return pos;
}

void startPathfinding(Maze& maze, const MoveSet moveSet) {
    std::random_device rd;
    std::mt19937 gen(rd());
    const std::uniform_int_distribution<int> xDistrib(1, maze[0].size() - 2);
    const std::uniform_int_distribution<int> yDistrib(1, maze.size() - 2);

    const Position start = getValidPosition(xDistrib, yDistrib, gen, maze);

    Position end{};
    do {
        end = getValidPosition(xDistrib, yDistrib, gen, maze);
    } while (end == start);

    std::cout << "BFS: \n";
    const std::optional<Path> bfsPath = findShortestPathBFS(start, end, maze, moveSet);

    drawPath(maze, start, end, bfsPath);
    printMaze(maze);
}

int main() {
    Maze maze = baseMaze;
    startPathfinding(maze, FourDirection);

    return 0;
}
