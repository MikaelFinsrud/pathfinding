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

constexpr std::array<int, 4> xOffsets{-1, 1, 0, 0};
constexpr std::array<int, 4> yOffsets{0, 0, -1, 1};

std::vector<Position> getAvailableNeighbours(Position pos, const std::vector<std::string>& maze);
void findShortestPathBFS(Position start, Position end, std::vector<std::string>& maze);

#endif //PATHFINDING_PATHFINDINGALGORITHMS_H
