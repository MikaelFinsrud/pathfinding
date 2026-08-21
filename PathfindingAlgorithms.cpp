//
// Created by mikke on 8/21/26.
//
#include <algorithm>
#include <iostream>
#include "PathfindingAlgorithms.h"
#include <limits>
#include <optional>
#include <queue>

namespace {
    template <typename GetPrevious>
    Path reconstructPath(const Position start, const Position end, GetPrevious getPrevious) {
        Path path;
        Position currentPos = end;

        while (currentPos != start) {
            path.push_back(currentPos);
            currentPos = getPrevious(currentPos);
        }

        path.push_back(start);

        return path;
    }

    struct NodeData {
        bool visited = false;
        Position previous;
        int distance = std::numeric_limits<int>::max();
    };

    struct QueueEntry {
        Position pos;
        int distance;
    };

    struct CompareQueueEntry {
        bool operator()(const QueueEntry& a, const QueueEntry& b) const {
            return a.distance > b.distance;
        }
    };

    bool isDiagonal(const Position& a, const Position& b) {
        return std::abs(static_cast<int>(a.x) - static_cast<int>(b.x)) == 1 &&
               std::abs(static_cast<int>(a.y) - static_cast<int>(b.y)) == 1;
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

std::optional<Path> findShortestPathDijkstra(const Position start, const Position end, const Maze& maze, const MoveSet moveSet, const bool verbose = true) {
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, CompareQueueEntry> toVisitQueue;
    std::vector<std::vector<NodeData>> nodeData(maze.size(), std::vector<NodeData>(maze[0].size()));

    nodeData[start.y][start.x].distance = 0;
    toVisitQueue.push({start, 0});
    bool foundTarget = false;

    while (!toVisitQueue.empty()) {
        QueueEntry current = toVisitQueue.top();
        toVisitQueue.pop();

        if (current.pos == end) {
            foundTarget = true;
            break;
        }

        if (current.distance > nodeData[current.pos.y][current.pos.x].distance) { // Queue might contain several entries of same node
            continue;
        }

        const std::vector<Position> neighbours = getAvailableNeighbours(current.pos, maze, moveSet);

        for (Position neighbour : neighbours) {
            if (nodeData[neighbour.y][neighbour.x].visited) {
                continue;
            }

            int weight = 10;

            if (isDiagonal(current.pos, neighbour)) {
                weight = 14;
            }

            const int newDistance = nodeData[current.pos.y][current.pos.x].distance + weight;
            const int currentDistance = nodeData[neighbour.y][neighbour.x].distance;

            if (newDistance < currentDistance) {
                nodeData[neighbour.y][neighbour.x].distance = newDistance;
                nodeData[neighbour.y][neighbour.x].previous = current.pos;

                toVisitQueue.push({.pos = neighbour, .distance = nodeData[neighbour.y][neighbour.x].distance});
            }
        }

        nodeData[current.pos.y][current.pos.x].visited = true;
    }

    if (!foundTarget) {
        if (verbose) {
            std::cerr << "ERROR: Could not join the two paths together!\n";
        }

        return std::nullopt;
    }

    Path path = reconstructPath(start,end,[&](const Position pos) {
        return nodeData[pos.y][pos.x].previous;
    });

    return path;
}

std::optional<Path> findShortestPathBFS(const Position start, const Position end, const Maze& maze, const MoveSet moveSet, const bool verbose = true) {

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
        if (verbose) {
            std::cerr << "ERROR: Could not join the two paths together!\n";
        }

        return std::nullopt;
    }

    Path path = reconstructPath(start,end,[&](const Position pos) {
        return previous[pos.y][pos.x];
    });

    return path;
}
