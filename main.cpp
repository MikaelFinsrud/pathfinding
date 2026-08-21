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
#include <chrono>
#include "Position.h"
#include "PathfindingAlgorithms.h"

const Maze baseMaze {
    "+--------------------+--------------------+--------------------+",
    "|....|...............---|...|--.--|.....---|...|--.--|.........|",
    "|.---|-.----.--|-....---|...|--.--|.....---|...|--.--|.........|",
    "|....|...|...........---|...|--.--|.....---|...|--.--|.........|",
    "|.---|...|--.--|.....---|...|--.--|.....---|...|--.--|.........|",
    "|..............|.......................................|.......|",
    "|....|...............---|...|--.--|.....---|...|--.--|.........|",
    "|.---|-.----.--|-....---|...|--.--|.....---|...|--.--|.........|",
    "|....|...|...........---|...|--.--|.....---|...|--.--|.........|",
    "|.---|...|--.--|.....---|...|--.--|.....---|...|--.--|.........|",
    "|..............|.......................................|.......|",
    "|....|...............---|...|--.--|.....---|...|--.--|.........|",
    "|.---|-.----.--|-....---|...|--.--|.....---|...|--.--|.........|",
    "|....|...|...........---|...|--.--|.....---|...|--.--|.........|",
    "|.---|...|--.--|.....---|...|--.--|.....---|...|--.--|.........|",
    "|..............|.......................................|.......|",
    "|....|...............---|...|--.--|.....---|...|--.--|.........|",
    "|.---|-.----.--|-....---|...|--.--|.....---|...|--.--|.........|",
    "|....|...|...........---|...|--.--|.....---|...|--.--|.........|",
    "|.---|...|--.--|.....---|...|--.--|.....---|...|--.--|.........|",
    "|..............|...................|...................|.......|",
    "+--------------------------------------------------------------+",
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

template <typename Algorithm>
void startPathfinding(Maze& maze, const MoveSet moveSet, Algorithm algorithm, const bool verbose) {
    std::random_device rd;
    std::mt19937 gen(rd());
    const std::uniform_int_distribution<int> xDistrib(1, maze[0].size() - 2);
    const std::uniform_int_distribution<int> yDistrib(1, maze.size() - 2);

    const Position start = getValidPosition(xDistrib, yDistrib, gen, maze);

    Position end{};
    do {
        end = getValidPosition(xDistrib, yDistrib, gen, maze);
    } while (end == start);

    const std::optional<Path> bfsPath = algorithm(start, end, maze, moveSet, verbose);

    if (verbose) {
        drawPath(maze, start, end, bfsPath);
        printMaze(maze);
    }
}

template <typename Algorithm>
void Benchmark(Maze& maze, const int iterations, MoveSet moveSet, Algorithm algorithm) {
    const auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < iterations; i++) {
        startPathfinding(maze, moveSet, algorithm, i==iterations-1);
    }

    const auto end = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "\tIterations: " << iterations << '\n';
    std::cout << "\tTime elapsed: " << elapsed.count() << '\n';
}

int main() {
    Maze maze = baseMaze;

    std::cout << "\nBFS:\n";
    Benchmark(maze, 1000, EightDirection, findShortestPathBFS);

    maze = baseMaze;
    std::cout << "\nDijkstra:\n";
    Benchmark(maze, 1000, EightDirection, findShortestPathDijkstra);

    return 0;
}
