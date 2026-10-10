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

bool LocalGame::isFiveAt(int row, int col, PieceType type) const {
    if (type == PieceType::PIECE_EMPTY) {
        return false;
    }
    if (row < 1 || row > boardSize || col < 1 || col > boardSize) {
        return false;
    }

    const PieceType cell = boardState[row][col];
    if (cell != PieceType::PIECE_EMPTY && cell != type) {
        return false;
    }

    const int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

    for (int d = 0; d < 4; ++d) {
        const int dr = directions[d][0];
        const int dc = directions[d][1];
        int cnt = 1;

        for (int step = 1; step <= boardSize; ++step) {
            const int r = row + dr * step;
            const int c = col + dc * step;
            if (r < 1 || r > boardSize || c < 1 || c > boardSize) {
                break;
            }
            if (boardState[r][c] != type) {
                break;
            }
            cnt += 1;
        }

        for (int step = 1; step <= boardSize; ++step) {
            const int r = row - dr * step;
            const int c = col - dc * step;
            if (r < 1 || r > boardSize || c < 1 || c > boardSize) {
                break;
            }
            if (boardState[r][c] != type) {
                break;
            }
            cnt += 1;
        }

        if (cnt >= 5) {
            return true;
        }
    }

    return false;
}

void LocalGame::checkOutcome() {
    const Move lastMove = getLastMove();
    if (lastMove.type == PieceType::PIECE_EMPTY) {
        return;
    }

    if (isFiveAt(lastMove.row, lastMove.col, lastMove.type)) {
        state = (lastMove.type == PieceType::PIECE_BLACK)
              ? LocalGameState::BLACK_WIN : LocalGameState::WHITE_WIN;
        return;
    }

    if (isBoardFull()) {
        state = LocalGameState::DRAW;
    }
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

    setCurrentPlayer(PieceType::PIECE_BLACK);
}

void LocalGame::setCurrentPlayer(PieceType color) {
    currentPlayer = &playerOf(color);
}

void LocalGame::nextTurn() {
    currentPlayer = (currentPlayer == &blackPlayer) ? &whitePlayer : &blackPlayer;
}

bool LocalGame::isPlaying() const {
    return state == LocalGameState::PLAYING;
}

LocalGameState LocalGame::getState() const {
    return state;
}

bool LocalGame::canUndo() const {
    return !moveHistory.empty();
}

bool LocalGame::canRedo() const {
    return !rmoveHistory.empty();
}

int LocalGame::getBoardSize() const {
    return boardSize;
}

PieceType LocalGame::getPiece(int row, int col) const {
    return boardState[row][col];
}

PieceType LocalGame::getCurrentPlaying() const {
    return currentPlayer->playing;
}

bool LocalGame::isBoardEmpty() const {
    return moveHistory.empty();
}

bool LocalGame::isBoardFull() const {
    return static_cast<int>(moveHistory.size()) >= boardSize * boardSize;
}

Player& LocalGame::playerOf(PieceType color) {
    return (color == PieceType::PIECE_WHITE) ? whitePlayer : blackPlayer;
}

const Player& LocalGame::playerOf(PieceType color) const {
    return (color == PieceType::PIECE_WHITE) ? whitePlayer : blackPlayer;
}

bool LocalGame::isPlayerAI(PieceType color) const {
    return playerOf(color).isAI();
}

bool LocalGame::isCurrentPlayerAI() const {
    return currentPlayer->isAI();
}

void LocalGame::setPlayerType(PieceType color, PlayerType type) {
    playerOf(color).type = type;
}

Move LocalGame::undoMove() {
    if (moveHistory.empty()) {
        return Move(0, 0, PieceType::PIECE_EMPTY);
    }

    const Move lastMove = moveHistory.top();
    moveHistory.pop();
    rmoveHistory.push(lastMove);

    boardState[lastMove.row][lastMove.col] = PieceType::PIECE_EMPTY;
    setCurrentPlayer(lastMove.type);
    state = LocalGameState::PLAYING;

    return lastMove;
}

Move LocalGame::redoMove() {
    if (rmoveHistory.empty()) {
        return Move(0, 0, PieceType::PIECE_EMPTY);
    }

    const Move move = rmoveHistory.top();
    rmoveHistory.pop();
    moveHistory.push(move);

    boardState[move.row][move.col] = move.type;
    setCurrentPlayer(move.type);
    nextTurn();
    state = LocalGameState::PLAYING;
    checkOutcome();

    return move;
}

bool LocalGame::saveToFile(const std::string& path) const {
    std::ofstream file(path);
    if (!file) {
        return false;
    }

    file << boardSize << '\n'
         << static_cast<int>(state) << '\n'
         << static_cast<int>(currentPlayer->playing) << '\n';

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

    file << static_cast<int>(blackPlayer.type) << ' '
         << static_cast<int>(whitePlayer.type) << '\n';

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

    int loadedBlackType = 0;
    int loadedWhiteType = 0;
    const bool hasPlayerTypes
        = static_cast<bool>(file >> loadedBlackType >> loadedWhiteType);

    boardSize = loadedBoardSize;
    state = static_cast<LocalGameState>(loadedState);
    setCurrentPlayer(static_cast<PieceType>(loadedPlayer));
    boardState = std::move(loadedBoard);
    moveHistory = std::move(loadedHistory);
    rmoveHistory = std::move(loadedRedoHistory);

    if (hasPlayerTypes) {
        blackPlayer.type = static_cast<PlayerType>(loadedBlackType);
        whitePlayer.type = static_cast<PlayerType>(loadedWhiteType);
    }

    return true;
}
