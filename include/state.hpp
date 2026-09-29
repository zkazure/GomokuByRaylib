#pragma once
#include <vector>
#include <stack>
#include <utility>
#include "type.hpp"
#include "raylib.h"

class GlobalGame {
public:
    Vector2 screenLayout = {800, 600};
    GameState state = GameState::INITIAL;
    Color boardBackground = GREEN;
    std::pair<int, int> score = {0, 0};

public:
    GlobalGame(Vector2 sl, GameState s);
    GlobalGame(Vector2 sl, GameState s, Color bbg);
};


struct Move {
    int row;
    int col;
    PieceType type;

    Move(int r, int c, PieceType t);
};


class LocalGame {
public:
    int boardSize = 15;
    std::vector<std::vector<PieceType>> boardState;
    std::stack<Move> moveHistory;

public:
    LocalGame();
    LocalGame(int bs);
    bool make1Move(Coordinate coor, PieceType type);
    Move getLastMove() const;
    PieceType checkOutcome() const;
};
