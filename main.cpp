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

struct Position {
    int x{}, y{};

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
    "|....P.....|",
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
        const int yPos = pos.y + yOffsets[i], xPos = pos.x + xOffsets[i];

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

void findShortestPath(const Position start, const Position end) {
    std::queue<Position> toVisitQueue;
    std::vector<Position> visitedPositions;
    std::vector<std::vector<Position>> previous(maze.size(), std::vector<Position>(maze[0].size())); // What position did I come from when I reached (x,y)?

    toVisitQueue.push(start);

    while (!toVisitQueue.empty()) {
        const Position current = toVisitQueue.front();
        toVisitQueue.pop();

        std::vector<Position> neighbours = getAvailableNeighbours(current);

        for (const Position& neighbour : neighbours) {
            if (std::find(visitedPositions.begin(), visitedPositions.end(), neighbour) == visitedPositions.end()) {
                toVisitQueue.push(neighbour);
                visitedPositions.push_back(neighbour); // Avoid getting queued again later
                previous[neighbour.y][neighbour.x] = current;
            }
        }
    }

    Position currentPos = end;
    int totalSteps = 0;

    while (currentPos != start) {
        ++totalSteps;
        maze[currentPos.y][currentPos.x] = '\\';
        currentPos = previous[currentPos.y][currentPos.x];
    }

    maze[start.y][start.x] = 'X'; maze[end.y][end.x] = 'X';

    std::cout << totalSteps << std::endl;
}



int main() {

    findShortestPath({.x = 4,.y = 5},{.x = 6, .y = 5});
    printMaze();

    return 0;
}
