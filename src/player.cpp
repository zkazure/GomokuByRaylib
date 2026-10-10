#include "player.hpp"
#include "type.hpp"

Player::Player(PieceType t, PlayerType k) : playing(t), type(k), score(0) {
    name = (t == PieceType::PIECE_WHITE) ? "player2" : "player1";
}

bool Player::isAI() const {
    return type == PlayerType::AI;
}
