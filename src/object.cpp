#include "object.hpp"
#include "raylib.h"

Object::Object() : position({0, 0}) {}
Object::Object(Vector2 p) : position(p) {}


Board::Board(Vector2 p, Vector2 outterLayout)
    : Object(p), layout({outterLayout.x*ratio, outterLayout.y*ratio}) {}
void Board::draw() {
    int realSize = size/10 * 600;
    const Vector2 origin = {position.x-width/2, position.y-width/2};
    DrawRectangleLines(origin.x, origin.y,
                       width, width, BLACK);

    float space_width = width / 15;
    for (int i = 1; i < 15; ++i) {
        DrawLine(origin.x + i*space_width, origin.y,
                 origin.x + i*space_width, origin.y + width,
                 BLACK);
        DrawLine(origin.x, origin.y + i*space_width,
                 origin.x + width, origin.y + i*space_width,
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
