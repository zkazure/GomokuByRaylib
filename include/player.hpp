#pragma once

#include "type.hpp"

struct Player {
    PieceType playing = PieceType::PIECE_EMPTY;

    Player(PieceType t);
};
