#pragma once
#include <vector>
#include "type.hpp"
#include "raylib.h"

class GlobalGame {
public:
    Vector2 screenLayout = {800, 600};
    GameState state = GameState::INITIAL;
    Color boardBackground = GREEN;

public:
    GlobalGame(Vector2 sl, GameState s);
    GlobalGame(Vector2 sl, GameState s, Color bbg);
};


class LocalGame {
public:
    int boardSize = 15;
    std::vector<std::vector<PieceType>> boardState;

public:
    LocalGame();
    LocalGame(int bs);
    bool make1Move(Coordinate coor, PieceType type);
};
