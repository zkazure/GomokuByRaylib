#include <iostream>
#include <limits>
#include "state.hpp"
#include "object.hpp"
#include "raylib.h"
#include "raymath.h"

Object::Object() : position({0, 0}) {}
Object::Object(Vector2 p) : position(p) {}


Board::Board(const GlobalGame * const g, const LocalGame * l, Vector2 p)
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

    // TOOD: init from localGame
    for (int i = 1; i <= boardSize; ++i) {
        for (int j = 1; j <= boardSize; ++j) {
            pieces[i][j] = new Piece(this, {leftTop.x + right.x * i, leftTop.y + down.y * j}, PieceType::PIECE_BLACK);
        }
    }
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
}

float Board::getWidth() const { return width; }
float Board::getCellWidth() const { return cellWidth; }


Piece::Piece(const Board *b, Vector2 p, PieceType t) : board(b), Object(p), type(t) {}
void Piece::draw() {
    if (type == PieceType::PIECE_BLACK) {
        DrawCircleV(position, radius, BLACK);
    } else {
        DrawCircleV(position, radius, WHITE);
    }
}

float Piece::getRadius() const { return radius; }
