#pragma once

#include <string>
#include "type.hpp"

struct Player {
    PieceType playing = PieceType::PIECE_EMPTY;
    PlayerType type = PlayerType::HUMAN;
    int score = 0;
    std::string name;

    explicit Player(PieceType t, PlayerType k = PlayerType::HUMAN);

    bool isAI() const;
};
