#include "player.hpp"
#include "type.hpp"

Player::Player(PieceType t, PlayerType k) : playing(t), type(k) {}

bool Player::isAI() const {
    return type == PlayerType::AI;
}
