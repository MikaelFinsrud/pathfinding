//
// Created by mikke on 8/20/26.
//
#include <iostream>
#include <ostream>
#include <vector>
#include <string>
#include <queue>
#include <array>

struct Position {
    int x, y;
};

const std::vector<std::string> maze {
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
        char c = maze[pos.y + yOffsets[i]][pos.x + xOffsets[i]];
        if (c == '.' || c == 'P') {
            neighbours.push_back(pos);
        }
    }

    return neighbours;
}

void findShortestPath(const Position start, const Position end) {
    std::queue<Position> toVisitQueue;
    std::vector<Position> visitedPositions;
    std::vector<std::vector<Position>> previous; // What position did I come from when I reached (x,y)?

    toVisitQueue.push(start);

    while (!toVisitQueue.empty()) {
        const Position current = toVisitQueue.front();
        std::vector<Position> neighbours = getAvailableNeighbours(current);

        for (const Position& neighbour : neighbours) {
            toVisitQueue.push(neighbour);
        }
    }

}



int main() {
    printMaze();

    return 0;
}
