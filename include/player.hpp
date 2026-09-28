#pragma once

#include "type.hpp"

class Player {
private:
    PieceType playing = PieceType::PIECE_EMPTY;

public:
    Player(PieceType t);
    PieceType getPlaying() const;
};
