#include "state.hpp"
#include "type.hpp"

GlobalGame::GlobalGame(Vector2 sl, GlobalGameState s)
    : screenLayout(sl), state(s) {}
GlobalGame::GlobalGame(Vector2 sl, GlobalGameState s, Color bbg)
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

    nextTurn();

    return true;
}

Move LocalGame::getLastMove() const {
    if (moveHistory.empty()) {
        return Move(0, 0, PieceType::PIECE_EMPTY);
    }

    return moveHistory.top();
}

void LocalGame::checkOutcome() {
    Move lastMove = getLastMove();
    if (lastMove.type == PieceType::PIECE_EMPTY) {
        return ;
    }

    {
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
            if (lastMove.type == PieceType::PIECE_BLACK) {
                state = LocalGameState::BLACK_WIN;
            } else {
                state = LocalGameState::WHITE_WIN;
            }
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
            if (lastMove.type == PieceType::PIECE_BLACK) {
                state = LocalGameState::BLACK_WIN;
            } else {
                state = LocalGameState::WHITE_WIN;
            }
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
            if (lastMove.type == PieceType::PIECE_BLACK) {
                state = LocalGameState::BLACK_WIN;
            } else {
                state = LocalGameState::WHITE_WIN;
            }
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
            if (lastMove.type == PieceType::PIECE_BLACK) {
                state = LocalGameState::BLACK_WIN;
            }                else {
                state = LocalGameState::WHITE_WIN;

            }
        }
    }



    return ;
}

void LocalGame::clear() {
    state = LocalGameState::PLAYING;

    for (int i = 1; i <= boardSize; ++i) {
        for (int j = 1; j <= boardSize; ++j) {
            boardState[i][j] = PieceType::PIECE_EMPTY;
        }
    }

    while (!moveHistory.empty()) {
        moveHistory.pop();
    }

    player.playing = PieceType::PIECE_BLACK;
}

void LocalGame::nextTurn() {
    if (player.playing == PieceType::PIECE_BLACK) {
        player.playing = PieceType::PIECE_WHITE;
    } else {
        player.playing = PieceType::PIECE_BLACK;
    }
}
