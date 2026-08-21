//
// Created by mikke on 8/21/26.
//
#include <algorithm>
#include <iostream>
#include "PathfindingAlgorithms.h"

std::vector<Position> getAvailableNeighbours(const Position pos, const std::vector<std::string>& maze) {
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

void findShortestPathBFS(const Position start, const Position end, std::vector<std::string>& maze) {

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

        std::vector<Position> neighbours = getAvailableNeighbours(current, maze);

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
