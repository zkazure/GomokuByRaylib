#pragma once
#include <utility>
#include "raylib.h"

enum class GlobalGameState { INITIAL, PLAYING, GAMEOVER };

enum class LocalGameState { PLAYING, BLACK_WIN, WHITE_WIN };

enum class PieceType {PIECE_EMPTY, PIECE_BLACK, PIECE_WHITE};

typedef std::pair<int, int> Coordinate;

namespace UiPalette {
    constexpr Color window = {247, 243, 232, 255};
    constexpr Color panel = {255, 252, 244, 255};
    constexpr Color ink = {76, 63, 47, 255};
    constexpr Color border = {200, 181, 143, 255};
    constexpr Color accent = {218, 165, 32, 255};
}
