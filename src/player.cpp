#include "player.hpp"
#include "type.hpp"

Player::Player(PieceType t) : playing(t) {}

PieceType Player::getPlaying() const { return playing; }
