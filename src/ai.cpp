#include "ai.hpp"

constexpr int ATTACK_WEIGHT = 11;
constexpr int DEFENSE_WEIGHT = 10;

enum PatternLevel {
    PATTERN_NONE,
    PATTERN_ONE,
    PATTERN_TWO,
    PATTERN_OPEN_TWO,
    PATTERN_THREE,
    PATTERN_OPEN_THREE,
    PATTERN_FOUR,
    PATTERN_OPEN_FOUR,
    PATTERN_FIVE
};

static int levelScore(PatternLevel level) {
    switch (level) {
        case PATTERN_FIVE: return 1000000;
        case PATTERN_OPEN_FOUR: return 100000;
        case PATTERN_FOUR: return 10000;
        case PATTERN_OPEN_THREE: return 8000;
        case PATTERN_THREE: return 800;
        case PATTERN_OPEN_TWO: return 400;
        case PATTERN_TWO: return 40;
        case PATTERN_ONE: return 4;
        default: return 0;
    }
}

static bool isOnBoard(const LocalGame& game, int row, int col) {
    const int boardSize = game.getBoardSize();
    return row >= 1 && row <= boardSize && col >= 1 && col <= boardSize;
}

static int runLength(const LocalGame& game, int row, int col, int dr, int dc,
                     PieceType type) {
    const int boardSize = game.getBoardSize();
    int len = 0;

    for (int step = 1; step <= boardSize; ++step) {
        const int r = row + dr * step;
        const int c = col + dc * step;
        if (!isOnBoard(game, r, c)) {
            break;
        }
        if (game.getPiece(r, c) != type) {
            break;
        }
        len += 1;
    }

    return len;
}

static bool isOpenEnd(const LocalGame& game, int row, int col, int dr, int dc, int run) {
    const int r = row + dr * (run + 1);
    const int c = col + dc * (run + 1);

    if (!isOnBoard(game, r, c)) {
        return false;
    }

    return game.getPiece(r, c) == PieceType::PIECE_EMPTY;
}

static PatternLevel levelAt(const LocalGame& game, int row, int col, int dr, int dc,
                            PieceType type) {
    const PieceType cell = game.getPiece(row, col);
    if (cell != PieceType::PIECE_EMPTY && cell != type) {
        return PATTERN_NONE;
    }

    const int forward = runLength(game, row, col, dr, dc, type);
    const int backward = runLength(game, row, col, -dr, -dc, type);
    const int len = 1 + forward + backward;

    int openEnds = 0;
    if (isOpenEnd(game, row, col, dr, dc, forward)) {
        openEnds += 1;
    }
    if (isOpenEnd(game, row, col, -dr, -dc, backward)) {
        openEnds += 1;
    }

    if (len >= 5) {
        return PATTERN_FIVE;
    }
    if (len == 4) {
        if (openEnds == 2) {
            return PATTERN_OPEN_FOUR;
        }
        if (openEnds == 1) {
            return PATTERN_FOUR;
        }
        return PATTERN_NONE;
    }
    if (len == 3) {
        if (openEnds == 2) {
            return PATTERN_OPEN_THREE;
        }
        if (openEnds == 1) {
            return PATTERN_THREE;
        }
        return PATTERN_NONE;
    }
    if (len == 2) {
        if (openEnds == 2) {
            return PATTERN_OPEN_TWO;
        }
        if (openEnds == 1) {
            return PATTERN_TWO;
        }
        return PATTERN_NONE;
    }

    return PATTERN_ONE;
}

static int pointScore(const LocalGame& game, int row, int col, PieceType type) {
    const int dr[4] = {0, 1, 1, 1};
    const int dc[4] = {1, 0, 1, -1};
    int total = 0;

    for (int d = 0; d < 4; ++d) {
        total += levelScore(levelAt(game, row, col, dr[d], dc[d], type));
    }

    return total;
}

static PieceType opponentOf(PieceType type) {
    if (type == PieceType::PIECE_BLACK) {
        return PieceType::PIECE_WHITE;
    }
    return PieceType::PIECE_BLACK;
}

static bool hasNeighbor(const LocalGame& game, int row, int col) {
    for (int dr = -AI::NEIGHBOR_RADIUS; dr <= AI::NEIGHBOR_RADIUS; ++dr) {
        for (int dc = -AI::NEIGHBOR_RADIUS; dc <= AI::NEIGHBOR_RADIUS; ++dc) {
            if (dr == 0 && dc == 0) {
                continue;
            }
            const int r = row + dr;
            const int c = col + dc;
            if (!isOnBoard(game, r, c)) {
                continue;
            }
            if (game.getPiece(r, c) != PieceType::PIECE_EMPTY) {
                return true;
            }
        }
    }

    return false;
}

static int centerDistance(const LocalGame& game, Coordinate move) {
    const int center = (game.getBoardSize() + 1) / 2;
    const int dr = move.first - center;
    const int dc = move.second - center;
    return ((dr < 0) ? -dr : dr) + ((dc < 0) ? -dc : dc);
}

static bool isBetterMove(const LocalGame& game, int score, Coordinate move,
                         int bestScore, Coordinate best) {
    if (score != bestScore) {
        return score > bestScore;
    }

    const int moveDistance = centerDistance(game, move);
    const int bestDistance = centerDistance(game, best);
    if (moveDistance != bestDistance) {
        return moveDistance < bestDistance;
    }
    if (move.first != best.first) {
        return move.first < best.first;
    }

    return move.second < best.second;
}

std::vector<Coordinate> AI::candidateMoves(const LocalGame& game) {
    const int boardSize = game.getBoardSize();
    const int center = (boardSize + 1) / 2;
    std::vector<Coordinate> moves;

    if (game.isBoardEmpty()) {
        moves.push_back(Coordinate(center, center));
        return moves;
    }

    for (int row = 1; row <= boardSize; ++row) {
        for (int col = 1; col <= boardSize; ++col) {
            if (game.getPiece(row, col) != PieceType::PIECE_EMPTY) {
                continue;
            }
            if (!hasNeighbor(game, row, col)) {
                continue;
            }
            moves.push_back(Coordinate(row, col));
        }
    }

    if (moves.empty()) {
        for (int row = 1; row <= boardSize; ++row) {
            for (int col = 1; col <= boardSize; ++col) {
                if (game.getPiece(row, col) == PieceType::PIECE_EMPTY) {
                    moves.push_back(Coordinate(row, col));
                }
            }
        }
    }

    return moves;
}

Coordinate AI::chooseMove(const LocalGame& game) {
    const std::vector<Coordinate> candidates = candidateMoves(game);
    if (candidates.empty()) {
        return Coordinate(0, 0);
    }

    const PieceType me = game.getCurrentPlaying();
    const PieceType opponent = opponentOf(me);
    const int count = static_cast<int>(candidates.size());

    for (int i = 0; i < count; ++i) {
        if (game.isFiveAt(candidates[i].first, candidates[i].second, me)) {
            return candidates[i];
        }
    }

    Coordinate block = Coordinate(0, 0);
    int blockAttack = 0;
    bool hasBlock = false;

    for (int i = 0; i < count; ++i) {
        if (!game.isFiveAt(candidates[i].first, candidates[i].second, opponent)) {
            continue;
        }
        const int attack = pointScore(game, candidates[i].first, candidates[i].second, me);
        if (!hasBlock || isBetterMove(game, attack, candidates[i], blockAttack, block)) {
            block = candidates[i];
            blockAttack = attack;
            hasBlock = true;
        }
    }

    if (hasBlock) {
        return block;
    }

    Coordinate best = candidates[0];
    int bestScore = ATTACK_WEIGHT * pointScore(game, best.first, best.second, me)
                  + DEFENSE_WEIGHT * pointScore(game, best.first, best.second, opponent);

    for (int i = 1; i < count; ++i) {
        const Coordinate move = candidates[i];
        const int score = ATTACK_WEIGHT * pointScore(game, move.first, move.second, me)
                        + DEFENSE_WEIGHT * pointScore(game, move.first, move.second, opponent);
        if (isBetterMove(game, score, move, bestScore, best)) {
            best = move;
            bestScore = score;
        }
    }

    return best;
}
