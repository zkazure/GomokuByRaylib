#pragma once
#include <utility>

enum class GameState { INITIAL, PLAYING, GAMEOVER };

enum class PieceType {PIECE_EMPTY, PIECE_BLACK, PIECE_WHITE};

typedef std::pair<int, int> Coordinate;
