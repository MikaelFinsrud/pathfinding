//
// Created by mikke on 8/21/26.
//

#ifndef PATHFINDING_POSITION_H
#define PATHFINDING_POSITION_H

struct Position {
    int x{}, y{};

    bool operator==(const Position&) const = default;
};

#endif //PATHFINDING_POSITION_H
