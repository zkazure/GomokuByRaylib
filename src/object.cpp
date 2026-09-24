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
    float space_width = width / boardSize;
    Vector2 leftTop = Vector2Add(position, {-width/2, -width/2});

    DrawRectangleLines(leftTop.x, leftTop.y, width, width, BLACK);

    for (int i = 1; i < boardSize; ++i) {
        DrawLineV(Vector2Add(leftTop, {0, i*space_width}),
                  Vector2Add(leftTop, {width, i*space_width}),
                  BLACK);

        DrawLineV(Vector2Add(leftTop, {i*space_width, 0}),
                  Vector2Add(leftTop, {i*space_width, width}),
                  BLACK);
    }
}


Piece::Piece(Vector2 p, PieceType t, float r) : Object(p), type(t), radius(r) {}
void Piece::draw() {
    if (type == PieceType::PIECE_BLACK) {
        DrawCircleV(position, radius, BLACK);
    } else {
        DrawCircleV(position, radius, WHITE);
    }
}
