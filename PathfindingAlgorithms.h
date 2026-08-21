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
#include <optional>

using Maze = std::vector<std::string>;
using Path = std::vector<Position>;

enum MoveSet {
    FourDirection = 4,
    EightDirection = 8,
};

std::vector<Position> getAvailableNeighbours(Position pos, const std::vector<std::string>& maze, MoveSet moveSet);
std::optional<std::vector<Position>> findShortestPathBFS(Position start, Position end, const std::vector<std::string>& maze, MoveSet moveSet);

#endif //PATHFINDING_PATHFINDINGALGORITHMS_H
