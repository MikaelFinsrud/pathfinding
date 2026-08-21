//
// Created by mikke on 8/21/26.
//

#ifndef PATHFINDING_POSITION_H
#define PATHFINDING_POSITION_H
#include <vector>

struct Position {
    std::size_t x{}, y{};

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Position& other) const {
        return x != other.x || y != other.y;
    }
};

#endif //PATHFINDING_POSITION_H
