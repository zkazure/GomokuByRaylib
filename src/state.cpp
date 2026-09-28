#include "state.hpp"
#include "type.hpp"

GlobalGame::GlobalGame(Vector2 sl, GameState s)
    : screenLayout(sl), state(s) {}
GlobalGame::GlobalGame(Vector2 sl, GameState s, Color bbg)
    : screenLayout(sl), state(s), boardBackground(bbg) {}


Move::Move(int r, int c, PieceType t) : row(r), col(c), type(t) {}


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
    moveHistory.push(Move(coor.first, coor.second, type));

    return true;
}

Move LocalGame::getLastMove() const {
    if (moveHistory.empty()) {
        return Move(0, 0, PieceType::PIECE_EMPTY);
    }

    return moveHistory.top();
}

PieceType LocalGame::checkOutcome() const {
    Move lastMove = getLastMove();
    if (lastMove.type == PieceType::PIECE_EMPTY) {
        return lastMove.type;
    }

    int cnt = 0;
    for (Coordinate nearMove = {lastMove.row, lastMove.col};
         nearMove.first > 0 && nearMove.first <= boardSize &&
             nearMove.second > 0 && nearMove.second <= boardSize;
         nearMove.first += 1) {
        if (boardState[nearMove.first][nearMove.second] == lastMove.type) {
            cnt += 1;
        }
    }
    for (Coordinate nearMove = {lastMove.row-1, lastMove.col};
         nearMove.first > 0 && nearMove.first <= boardSize &&
             nearMove.second > 0 && nearMove.second <= boardSize;
         nearMove.first -= 1) {
        if (boardState[nearMove.first][nearMove.second] == lastMove.type) {
            cnt += 1;
        }
    }
    if (cnt >= 5) {
        return lastMove.type;
    }

    cnt = 0;
    for (Coordinate nearMove = {lastMove.row, lastMove.col};
         nearMove.first > 0 && nearMove.first <= boardSize &&
             nearMove.second > 0 && nearMove.second <= boardSize;
         nearMove.second += 1) {
        if (boardState[nearMove.first][nearMove.second] == lastMove.type) {
            cnt += 1;
        }
    }
    for (Coordinate nearMove = {lastMove.row, lastMove.col-1};
         nearMove.first > 0 && nearMove.first <= boardSize &&
             nearMove.second > 0 && nearMove.second <= boardSize;
         nearMove.second -= 1) {
        if (boardState[nearMove.first][nearMove.second] == lastMove.type) {
            cnt += 1;
        }
    }
    if (cnt >= 5) {
        return lastMove.type;
    }

    cnt = 0;
    for (Coordinate nearMove = {lastMove.row, lastMove.col};
         nearMove.first > 0 && nearMove.first <= boardSize &&
             nearMove.second > 0 && nearMove.second <= boardSize;
         nearMove.first += 1, nearMove.second += 1) {
        if (boardState[nearMove.first][nearMove.second] == lastMove.type) {
            cnt += 1;
        }
    }
    for (Coordinate nearMove = {lastMove.row-1, lastMove.col-1};
         nearMove.first > 0 && nearMove.first <= boardSize &&
             nearMove.second > 0 && nearMove.second <= boardSize;
         nearMove.first -= 1, nearMove.second -= 1) {
        if (boardState[nearMove.first][nearMove.second] == lastMove.type) {
            cnt += 1;
        }
    }
    if (cnt >= 5) {
        return lastMove.type;
    }

    cnt = 0;
    for (Coordinate nearMove = {lastMove.row, lastMove.col};
         nearMove.first > 0 && nearMove.first <= boardSize &&
             nearMove.second > 0 && nearMove.second <= boardSize;
         nearMove.first += 1, nearMove.second -= 1) {
        if (boardState[nearMove.first][nearMove.second] == lastMove.type) {
            cnt += 1;
        }
    }
    for (Coordinate nearMove = {lastMove.row-1, lastMove.col+1};
         nearMove.first > 0 && nearMove.first <= boardSize &&
             nearMove.second > 0 && nearMove.second <= boardSize;
         nearMove.first -= 1, nearMove.second += 1) {
        if (boardState[nearMove.first][nearMove.second] == lastMove.type) {
            cnt += 1;
        }
    }
    if (cnt >= 5) {
        return lastMove.type;
    }

    return PieceType::PIECE_EMPTY;
}
