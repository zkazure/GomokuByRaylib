#pragma once

#include <vector>
#include "type.hpp"
#include "state.hpp"

namespace AI {
    constexpr int NEIGHBOR_RADIUS = 2;

    Coordinate chooseMove(const LocalGame& game);
    std::vector<Coordinate> candidateMoves(const LocalGame& game);
}
