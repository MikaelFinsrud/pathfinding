//
// Created by mikke on 8/21/26.
//

#ifndef PATHFINDING_PATHFINDINGALGORITHMS_H
#define PATHFINDING_PATHFINDINGALGORITHMS_H
#include <array>
#include <queue>
#include <vector>
#include <string>
#include "Position.h"

enum MoveSet {
    FourDirection = 4,
    EightDirection = 8,
};

constexpr std::array<int, 8> xOffsets{-1, 1, 0, 0, -1, -1, 1, 1};
constexpr std::array<int, 8> yOffsets{0, 0, -1, 1, -1, 1, -1, 1};

std::vector<Position> getAvailableNeighbours(Position pos, const std::vector<std::string>& maze, MoveSet moveSet);
void findShortestPathBFS(Position start, Position end, std::vector<std::string>& maze, MoveSet moveSet);

#endif //PATHFINDING_PATHFINDINGALGORITHMS_H
