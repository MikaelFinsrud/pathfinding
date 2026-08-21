//
// Created by mikke on 8/21/26.
//
#include <algorithm>
#include <iostream>
#include "PathfindingAlgorithms.h"

namespace {
    Path reconstructPath(const std::vector<std::vector<Position>>& previous, const Position start, const Position end) {
        Path path;
        Position currentPos = end;

        while (currentPos != start) {
            currentPos = previous[currentPos.y][currentPos.x];
            path.push_back(currentPos);
        }

        return path;
    }

    constexpr std::array<int, 8> xOffsets{-1, 1, 0, 0, -1, -1, 1, 1};
    constexpr std::array<int, 8> yOffsets{0, 0, -1, 1, -1, 1, -1, 1};
}

std::vector<Position> getAvailableNeighbours(const Position pos, const Maze& maze, const MoveSet moveSet) {
    std::vector<Position> neighbours;

    for (int i = 0; i < moveSet; i++) {
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

std::optional<Path> findShortestPathBFS(const Position start, const Position end, const Maze& maze, const MoveSet moveSet) {

    std::queue<Position> toVisitQueue;
    std::vector<std::vector<bool>> visitedPositions(maze.size(), std::vector<bool>(maze[0].size(), false));
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

        std::vector<Position> neighbours = getAvailableNeighbours(current, maze, moveSet);

        for (const Position& neighbour : neighbours) {
            if (!visitedPositions[neighbour.y][neighbour.x]) {
                toVisitQueue.push(neighbour);
                visitedPositions[neighbour.y][neighbour.x] = true; // Avoid getting queued again later
                previous[neighbour.y][neighbour.x] = current;
            }
        }
    }

    if (!foundTarget) {
        std::cerr << "ERROR: Could not join the two paths together!";
        return std::nullopt;
    }

    Path path = reconstructPath(previous, start, end);

    return path;
}
