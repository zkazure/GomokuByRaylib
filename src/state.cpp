#include "state.hpp"
#include "type.hpp"
#include <fstream>
#include <vector>

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
    while (!rmoveHistory.empty()) {
        rmoveHistory.pop();
    }

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
            if (boardState[nearMove.first][nearMove.second] != lastMove.type) {
                break;
            } else {
                cnt += 1;
            }
        }
        for (Coordinate nearMove = {lastMove.row-1, lastMove.col};
             nearMove.first > 0 && nearMove.first <= boardSize &&
                 nearMove.second > 0 && nearMove.second <= boardSize;
             nearMove.first -= 1) {
            if (boardState[nearMove.first][nearMove.second] != lastMove.type) {
                break;
            } else {
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
            if (boardState[nearMove.first][nearMove.second] != lastMove.type) {
                break;
            } else {
                cnt += 1;
            }
        }
        for (Coordinate nearMove = {lastMove.row, lastMove.col-1};
             nearMove.first > 0 && nearMove.first <= boardSize &&
                 nearMove.second > 0 && nearMove.second <= boardSize;
             nearMove.second -= 1) {
            if (boardState[nearMove.first][nearMove.second] != lastMove.type) {
                break;
            } else {
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
            if (boardState[nearMove.first][nearMove.second] != lastMove.type) {
                break;
            } else {
                cnt += 1;
            }
        }
        for (Coordinate nearMove = {lastMove.row-1, lastMove.col-1};
             nearMove.first > 0 && nearMove.first <= boardSize &&
                 nearMove.second > 0 && nearMove.second <= boardSize;
             nearMove.first -= 1, nearMove.second -= 1) {
            if (boardState[nearMove.first][nearMove.second] != lastMove.type) {
                break;
            } else {
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
            if (boardState[nearMove.first][nearMove.second] != lastMove.type) {
                break;
            } else {
                cnt += 1;
            }
        }
        for (Coordinate nearMove = {lastMove.row-1, lastMove.col+1};
             nearMove.first > 0 && nearMove.first <= boardSize &&
                 nearMove.second > 0 && nearMove.second <= boardSize;
             nearMove.first -= 1, nearMove.second += 1) {
            if (boardState[nearMove.first][nearMove.second] != lastMove.type) {
                break;
            } else {
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
    while (!rmoveHistory.empty()) {
        rmoveHistory.pop();
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

bool LocalGame::saveToFile(const std::string& path) const {
    std::ofstream file(path);
    if (!file) {
        return false;
    }

    file << boardSize << '\n'
         << static_cast<int>(state) << '\n'
         << static_cast<int>(player.playing) << '\n';

    for (int row = 1; row <= boardSize; ++row) {
        for (int col = 1; col <= boardSize; ++col) {
            file << static_cast<int>(boardState[row][col]) << ' ';
        }
        file << '\n';
    }

    std::stack<Move> history = moveHistory;
    std::vector<Move> moves;
    while (!history.empty()) {
        moves.push_back(history.top());
        history.pop();
    }

    file << moves.size() << '\n';
    for (auto move = moves.rbegin(); move != moves.rend(); ++move) {
        file << move->row << ' ' << move->col << ' '
             << static_cast<int>(move->type) << '\n';
    }

    std::stack<Move> redoHistory = rmoveHistory;
    moves.clear();
    while (!redoHistory.empty()) {
        moves.push_back(redoHistory.top());
        redoHistory.pop();
    }

    file << moves.size() << '\n';
    for (auto move = moves.rbegin(); move != moves.rend(); ++move) {
        file << move->row << ' ' << move->col << ' '
             << static_cast<int>(move->type) << '\n';
    }

    return static_cast<bool>(file);
}

bool LocalGame::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        return false;
    }

    int loadedBoardSize = 0;
    int loadedState = 0;
    int loadedPlayer = 0;
    file >> loadedBoardSize >> loadedState >> loadedPlayer;

    std::vector<std::vector<PieceType>> loadedBoard(
        loadedBoardSize + 2,
        std::vector<PieceType>(loadedBoardSize + 2, PieceType::PIECE_EMPTY));
    for (int row = 1; row <= loadedBoardSize; ++row) {
        for (int col = 1; col <= loadedBoardSize; ++col) {
            int pieceType = 0;
            file >> pieceType;
            loadedBoard[row][col] = static_cast<PieceType>(pieceType);
        }
    }

    std::size_t moveCount = 0;
    file >> moveCount;
    std::stack<Move> loadedHistory;
    for (std::size_t i = 0; i < moveCount; ++i) {
        int row = 0;
        int col = 0;
        int pieceType = 0;
        file >> row >> col >> pieceType;
        loadedHistory.push(Move(row, col, static_cast<PieceType>(pieceType)));
    }

    std::stack<Move> loadedRedoHistory;
    std::size_t redoMoveCount = 0;
    if (file >> redoMoveCount) {
        for (std::size_t i = 0; i < redoMoveCount; ++i) {
            int row = 0;
            int col = 0;
            int pieceType = 0;
            file >> row >> col >> pieceType;
            loadedRedoHistory.push(
                Move(row, col, static_cast<PieceType>(pieceType)));
        }
    }

    boardSize = loadedBoardSize;
    state = static_cast<LocalGameState>(loadedState);
    player.playing = static_cast<PieceType>(loadedPlayer);
    boardState = std::move(loadedBoard);
    moveHistory = std::move(loadedHistory);
    rmoveHistory = std::move(loadedRedoHistory);
    return true;
}
