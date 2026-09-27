#include "state.hpp"
#include "type.hpp"

GlobalGame::GlobalGame(Vector2 sl, GameState s)
    : screenLayout(sl), state(s) {}
GlobalGame::GlobalGame(Vector2 sl, GameState s, Color bbg)
    : screenLayout(sl), state(s), boardBackground(bbg) {}


LocalGame::LocalGame() : boardSize(15) {
    boardState.resize(boardSize+2);
    for (int i = 0; i <= boardSize+1; ++i) {
        boardState[i].resize(boardSize+2);
        for (int j = 0; j <= boardSize+1; ++j) {
            boardState[i][j] = PieceType::PIECE_EMPTY;
        }
    }
}
LocalGame::LocalGame(int bs) : boardSize(bs) {
    boardState.resize(boardSize+2);
    for (int i = 0; i <= boardSize+1; ++i) {
        boardState[i].resize(boardSize+2);
        for (int j = 0; j <= boardSize+1; ++j) {
            boardState[i][j] = PieceType::PIECE_EMPTY;
        }
    }
}

bool LocalGame::make1Move(Coordinate coor, PieceType type) {
    if (boardState[coor.first][coor.second] != PieceType::PIECE_EMPTY) {
        return false;
    }

    boardState[coor.first][coor.second] = type;

    return true;
}
