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

struct Position {
    std::size_t x{}, y{};

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Position& other) const {
        return x != other.x || y != other.y;
    }
};

std::vector<std::string> maze {
    "+----------+",
    "|....|.....|",
    "|....|.....|",
    "|....|.....|",
    "|....|.....|",
    "|....|.....|",
    "+----------+",
};

constexpr std::array<int, 4> xOffsets{-1, 1, 0, 0};
constexpr std::array<int, 4> yOffsets{0, 0, -1, 1};

void printMaze() {
    for (const std::string& s : maze) {
        std::cout << s << std::endl;
    }
}

std::vector<Position> getAvailableNeighbours(const Position pos) {
    std::vector<Position> neighbours;

    for (int i = 0; i < xOffsets.size(); i++) {
        const std::size_t yPos = pos.y + yOffsets[i], xPos = pos.x + xOffsets[i];

        if (yPos >= maze.size() || yPos < 0 ||  xPos >= maze[0].size() || xPos < 0) {
            continue;
        }

        const char c = maze[yPos][xPos];
        if (c == '.' || c == 'P') {
            neighbours.push_back({.x = xPos, .y= yPos});
        }
    }

    return neighbours;
}

void findShortestPathBFS(const Position start, const Position end) {

    std::queue<Position> toVisitQueue;
    std::vector<Position> visitedPositions;
    std::vector<std::vector<Position>> previous(maze.size(), std::vector<Position>(maze[0].size())); // What position did I come from when I reached (x,y)?

    toVisitQueue.push(start);
    bool foundTarget = false;

    while (!toVisitQueue.empty()) {
        const Position current = toVisitQueue.front();
        toVisitQueue.pop();

        if (current == end) {
            foundTarget = true;
            break;
        }

        std::vector<Position> neighbours = getAvailableNeighbours(current);

        for (const Position& neighbour : neighbours) {
            if (std::find(visitedPositions.begin(), visitedPositions.end(), neighbour) == visitedPositions.end()) {
                toVisitQueue.push(neighbour);
                visitedPositions.push_back(neighbour); // Avoid getting queued again later
                previous[neighbour.y][neighbour.x] = current;
            }
        }
    }

    maze[start.y][start.x] = 'X'; maze[end.y][end.x] = 'X';

    if (!foundTarget) {
        std::cerr << "ERROR: Could not join the two paths together!";
        return;
    }

    Position currentPos = end;
    int totalSteps = 0;

    while (currentPos != start) {
        ++totalSteps;
        maze[currentPos.y][currentPos.x] = '\\';
        currentPos = previous[currentPos.y][currentPos.x];
    }

    maze[start.y][start.x] = 'X'; maze[end.y][end.x] = 'X';
}

Position getValidPosition(std::uniform_int_distribution<std::size_t> xDistrib, std::uniform_int_distribution<std::size_t> yDistrib, std::mt19937& gen) {
    Position pos;

    do {
        pos.x = xDistrib(gen);
        pos.y = yDistrib(gen);
    } while (maze[pos.y][pos.x] != '.');

    return pos;
}

void startPathfinding() {
    std::random_device rd;
    std::mt19937 gen(rd());
    const std::uniform_int_distribution<std::size_t> xDistrib(1, maze[0].size() - 2);
    const std::uniform_int_distribution<std::size_t> yDistrib(1, maze.size() - 2);

    const Position start = getValidPosition(xDistrib, yDistrib, gen);

    Position end{};
    do {
        std::cout << end.x << end.y << std::endl;
        end = getValidPosition(xDistrib, yDistrib, gen);
    } while (end == start);

    std::cout << "BFS: \n";
    findShortestPathBFS(start, end);
    printMaze();
}


int main() {
    startPathfinding();

    return 0;
}
