#include "state.hpp"
#include "object.hpp"
#include "raylib.h"
#include "raymath.h"

Object::Object() : position({0, 0}) {}
Object::Object(Vector2 p) : position(p) {}


Board::Board(const GlobalGame * const g, const LocalGame * l, Vector2 p)
    : Object(p), globalGame(g), localGame(l) {}

void Board::draw() {
    int boardSize = localGame->boardSize;
    Vector2 leftTop = Vector2Add(position, {-width/2, -width/2});
    Color background = globalGame->boardBackground;

    DrawRectangleV(leftTop, {width, width}, background);
    DrawRectangleLines(leftTop.x, leftTop.y, width, width, BLACK);

    for (int i = 1; i < boardSize; ++i) {
        DrawLineV(Vector2Add(leftTop, {0, i*cellWidth}),
                  Vector2Add(leftTop, {width, i*cellWidth}),
                  BLACK);

        DrawLineV(Vector2Add(leftTop, {i*cellWidth, 0}),
                  Vector2Add(leftTop, {i*cellWidth, width}),
                  BLACK);
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
