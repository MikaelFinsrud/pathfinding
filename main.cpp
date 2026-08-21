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


const std::vector<std::string> baseMaze {
    "+----------+",
    "|....|.....|",
    "|....|.....|",
    "|....|.....|",
    "|....|.....|",
    "|....|.....|",
    "+----------+",
};

void printMaze(const std::vector<std::string>& maze) {
    for (const std::string& s : maze) {
        std::cout << s << std::endl;
    }
}

Position getValidPosition(std::uniform_int_distribution<std::size_t> xDistrib, std::uniform_int_distribution<std::size_t> yDistrib, std::mt19937& gen, const std::vector<std::string>& maze) {
    Position pos;

    do {
        pos.x = xDistrib(gen);
        pos.y = yDistrib(gen);
    } while (maze[pos.y][pos.x] != '.');

    return pos;
}

void startPathfinding(std::vector<std::string>& maze, const MoveSet moveSet) {
    std::random_device rd;
    std::mt19937 gen(rd());
    const std::uniform_int_distribution<std::size_t> xDistrib(1, maze[0].size() - 2);
    const std::uniform_int_distribution<std::size_t> yDistrib(1, maze.size() - 2);

    const Position start = getValidPosition(xDistrib, yDistrib, gen, maze);

    Position end{};
    do {
        std::cout << end.x << end.y << std::endl;
        end = getValidPosition(xDistrib, yDistrib, gen, maze);
    } while (end == start);

    std::cout << "BFS: \n";
    findShortestPathBFS(start, end, maze, moveSet);
    printMaze(maze);
}

int main() {
    std::vector<std::string> maze = baseMaze;
    startPathfinding(maze, FourDirection);

    return 0;
}
