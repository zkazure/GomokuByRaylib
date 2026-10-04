#pragma once
#include <utility>
#include "raylib.h"

enum class GlobalGameState { INITIAL, PLAYING, GAMEOVER };

enum class LocalGameState { PLAYING, BLACK_WIN, WHITE_WIN };

enum class PieceType {PIECE_EMPTY, PIECE_BLACK, PIECE_WHITE};

typedef std::pair<int, int> Coordinate;

namespace UiPalette {
    constexpr Color window = {246, 246, 242, 255};
    constexpr Color panel = {255, 255, 255, 255};
    constexpr Color ink = {24, 24, 24, 255};
    constexpr Color disabled = {224, 224, 224, 255};
    constexpr Color disabledInk = {130, 130, 130, 255};
    constexpr Color border = {24, 24, 24, 255};
    constexpr Color accent = {220, 38, 38, 255};
    constexpr Color board = {210, 210, 202, 255};
    constexpr Color muted = {105, 105, 100, 255};
}
