#pragma once
#include <utility>

enum class GlobalGameState { INITIAL, PLAYING, GAMEOVER };

enum class LocalGameState { PALYING, BLACK_WIN, WHITE_WIN };

enum class PieceType {PIECE_EMPTY, PIECE_BLACK, PIECE_WHITE};

typedef std::pair<int, int> Coordinate;
