#pragma once
#include <vector>
#include <stack>
#include <string>
#include <utility>
#include "type.hpp"
#include "player.hpp"
#include "raylib.h"

class GlobalGame {
public:
    Vector2 screenLayout = {800, 600};
    GlobalGameState state = GlobalGameState::INITIAL;
    Color boardBackground = BEIGE;
    std::pair<int, int> score = {0, 0};

public:
    GlobalGame(Vector2 sl, GlobalGameState s);
    GlobalGame(Vector2 sl, GlobalGameState s, Color bbg);
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
    LocalGameState state = LocalGameState::PLAYING;
    Player blackPlayer = Player(PieceType::PIECE_BLACK, PlayerType::HUMAN);
    Player whitePlayer = Player(PieceType::PIECE_WHITE, PlayerType::HUMAN);
    Player *currentPlayer = &blackPlayer;
    std::vector<std::vector<PieceType>> boardState;
    std::stack<Move> moveHistory;
    std::stack<Move> rmoveHistory;

public:
    LocalGame();
    LocalGame(int bs);
    bool make1Move(Coordinate coor, PieceType type);
    Move getLastMove() const;
    void checkOutcome();
    void clear();
    void setCurrentPlayer(PieceType color);
    void nextTurn();
    bool saveToFile(const std::string& path) const;
    bool loadFromFile(const std::string& path);
};
