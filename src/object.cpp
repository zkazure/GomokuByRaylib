#include <iostream>
#include <limits>
#include <cmath>
#include "state.hpp"
#include "object.hpp"
#include "raylib.h"
#include "raymath.h"
#include "type.hpp"

Object::Object() : position({0, 0}) {}
Object::Object(Vector2 p) : position(p) {}


Board::Board(GlobalGame * const g, LocalGame * l, Vector2 p)
    : Object(p), globalGame(g), localGame(l)
{
    int boardSize = localGame->boardSize;

    for (int i = 0; i <= boardSize+1; ++i) {
        intersections.push_back(std::vector<Vector2>());
        intersections[i].resize(boardSize+2);

        pieces.push_back(std::vector<Piece *>());
        pieces[i].resize(boardSize+2);
    }

    float inf = std::numeric_limits<float>::infinity();
    Vector2 right = {cellWidth, 0}, down = {0, cellWidth};

    intersections[0][0] = {-inf, -inf};
    intersections[boardSize+1][boardSize+1] = {inf, inf};
    intersections[0][boardSize+1] = {inf, -inf};
    intersections[boardSize+1][0] = {-inf, inf};

    for (int i = 1; i <= boardSize; ++i) {
        intersections[0][i] = {leftTop.x + right.x * i, -inf};
        intersections[boardSize+1][i] = {leftTop.x + right.x * i, inf};
        intersections[i][0] = {-inf, leftTop.y + down.y * i};
        intersections[i][boardSize+1] = {inf, leftTop.y + down.y * i};
    }

    for (int i = 1; i <= boardSize; ++i) {
        for (int j = 1; j <= boardSize; ++j) {
            intersections[i][j] =
                Vector2Add(leftTop,
                           Vector2Add(Vector2Multiply({0, (float)i}, down),
                                      Vector2Multiply({(float)j, 0}, right)));
        }
    }

    // // TOOD: init from localGame
    // for (int i = 1; i <= boardSize; ++i) {
    //     for (int j = 1; j <= boardSize; ++j) {
    //         pieces[i][j] = new Piece(this, {leftTop.x + right.x * i, leftTop.y + down.y * j}, PieceType::PIECE_BLACK);
    //     }
    // }
}

Board::~Board() {
    int boardSize = localGame->boardSize;
    for (int i = 1; i <= boardSize; ++i) {
        for (int j = 1; j <= boardSize; ++j) {
            if (pieces[i][j] != nullptr) {
                delete pieces[i][j];
            }
        }
    }
}

void Board::draw() {
    int boardSize = localGame->boardSize;
    Color background = globalGame->boardBackground;

    DrawRectangleV(leftTop, {width, width}, background);
    DrawRectangleLines(leftTop.x, leftTop.y, width, width, BLACK);

    for (int i = 1; i <= boardSize; ++i) {
        DrawLineV(Vector2Add(intersections[1][i], {0, -cellWidth}),
                  Vector2Add(intersections[boardSize][i], {0, cellWidth}),
                  BLACK);
        DrawLineV(Vector2Add(intersections[i][1], {-cellWidth, 0}),
                  Vector2Add(intersections[i][boardSize], {cellWidth, 0}),
                  BLACK);
    }

    for (int i = 1; i <= boardSize; ++i) {
        for (int j = 1; j <= boardSize; ++j) {
            if (pieces[i][j] != nullptr) {
                pieces[i][j]->draw();
            }
        }
    }

    Move lastMove = localGame->getLastMove();
    if (lastMove.type != PieceType::PIECE_EMPTY) {
        float pieceRadius = cellWidth * (float)1/7;
        DrawCircleV(coor2pos({lastMove.row, lastMove.col}),
                   pieceRadius, GOLD);
    }
}

float Board::getWidth() const { return width; }
float Board::getCellWidth() const { return cellWidth; }

Coordinate Board::pos2coor(Vector2 position) const {
    Coordinate coordinate = {16, 16};

    int boardSize = localGame->boardSize;
    for (int i = 0; i <= boardSize+1; ++i) {
        if (position.x < intersections[0][i].x) {
            coordinate.second = i-1;
            break;
        }
    }
    for (int i = 0; i <= boardSize+1; ++i) {
        if (position.y < intersections[i][0].y) {
            coordinate.first = i-1;
            break;
        }
    }

    {
        float distance = cellWidth;
        Coordinate tempCoordination = coordinate;
        float tempDistance
            = Vector2Distance(position, intersections[tempCoordination.first][tempCoordination.second]);
        if (tempDistance < distance) {
            coordinate = tempCoordination;
            distance = tempDistance;
        }

        tempCoordination.first += 1;
        tempDistance
            = Vector2Distance(position, intersections[tempCoordination.first][tempCoordination.second]);
        if (tempDistance < distance) {
            coordinate = tempCoordination;
            distance = tempDistance;
        }

        tempCoordination.second += 1;
        tempDistance
            = Vector2Distance(position, intersections[tempCoordination.first][tempCoordination.second]);
        if (tempDistance < distance) {
            coordinate = tempCoordination;
            distance = tempDistance;
        }

        tempCoordination.first -= 1;
        tempDistance
            = Vector2Distance(position, intersections[tempCoordination.first][tempCoordination.second]);
        if (tempDistance < distance) {
            coordinate = tempCoordination;
            distance = tempDistance;
        }
    }

    return coordinate;
}

Vector2 Board::coor2pos(Coordinate coor) const {
    return intersections[coor.first][coor.second];
}

bool Board::createPiece(Coordinate coor, PieceType type) {
    const int boardSize = localGame->boardSize;
    if (coor.first < 1 || coor.first > boardSize
        || coor.second < 1 || coor.second > boardSize) {
        return false;
    }

    if (!localGame->make1Move(coor, type)) {
        return false;
    }

    pieces[coor.first][coor.second] = new Piece(this, coor2pos(coor), type);

    return true;
}

bool Board::createPiece(Vector2 position, PieceType type) {
    const int boardSize = localGame->boardSize;
    if (!std::isfinite(position.x) || !std::isfinite(position.y)
        || position.x < intersections[1][1].x
        || position.x > intersections[boardSize][boardSize].x
        || position.y < intersections[1][1].y
        || position.y > intersections[boardSize][boardSize].y) {
        return false;
    }

    return createPiece(pos2coor(position), type);
}

PieceType Board::regret() {
    if (localGame->moveHistory.empty()) {
        return PieceType::PIECE_EMPTY;
    }

    Move lastMove = localGame->moveHistory.top();
    localGame->moveHistory.pop();

    Piece*& lastPiece = pieces[lastMove.row][lastMove.col];
    PieceType type = lastPiece->getType();

    delete lastPiece;
    lastPiece = nullptr;

    localGame->boardState[lastMove.row][lastMove.col] = PieceType::PIECE_EMPTY;

    localGame->player = type;

    if (globalGame->state == GlobalGameState::GAMEOVER) {
        const LocalGameState outcome = localGame->state;

        if (outcome == LocalGameState::BLACK_WIN && globalGame->score.first > 0) {
            globalGame->score.first -= 1;
        } else if (outcome == LocalGameState::WHITE_WIN && globalGame->score.second > 0) {
            globalGame->score.second -= 1;
        }
    }

    localGame->state = LocalGameState::PLAYING;
    globalGame->state = GlobalGameState::PLAYING;

    return type;
}

void Board::clear() {
    int size = localGame->boardSize;

    for (int i = 1; i <= size; ++i) {
        for (int j = 1; j <= size; ++j) {
            if (pieces[i][j] != nullptr) {
                delete pieces[i][j];
                pieces[i][j] = nullptr;
            }
        }
    }

    localGame->clear();
}

Piece::Piece(const Board *b, Vector2 p, PieceType t) : board(b), Object(p), type(t) {}
void Piece::draw() {
    if (type == PieceType::PIECE_BLACK) {
        DrawCircleV(position, radius, BLACK);
    } else {
        DrawCircleV(position, radius, WHITE);
    }
}

float Piece::getRadius() const { return radius; }

PieceType Piece::getType() const { return type; }
