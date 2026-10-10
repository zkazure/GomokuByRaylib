#pragma once

#include "type.hpp"

struct Player {
    PieceType playing = PieceType::PIECE_EMPTY;
    PlayerType type = PlayerType::HUMAN;

    explicit Player(PieceType t, PlayerType k = PlayerType::HUMAN);

    bool isAI() const;
};
