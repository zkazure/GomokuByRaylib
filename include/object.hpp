#pragma once

#include <vector>
#include <algorithm>
#include "raylib.h"
#include "raymath.h"
#include "state.hpp"

class Object {
protected:
    Vector2 position;

public:
    Object();
    Object(Vector2 p);
    virtual void draw() = 0;
};


class Board : public Object {
private:
    const GlobalGame * const globalGame;
    const LocalGame *localGame;
    const float ratio = (float)3/4;
    const float width = ratio * std::min(globalGame->screenLayout.x, globalGame->screenLayout.y);
    const float cellWidth = width / (localGame->boardSize + 1);
    const Vector2 leftTop = Vector2Add(position, {-width/2, -width/2});
    std::vector<std::vector<Vector2>> intersections;

public:
    Board(const GlobalGame * const globalGame, const LocalGame * localGame, Vector2 p);
    void draw() override;
    float getWidth() const ;
    float getCellWidth() const;
};

enum class PieceType {PIECE_NIL, PIECE_BLACK, PIECE_WHITE};

class Piece : public Object {
private:
    const Board * const board;
    PieceType type;
    const float ratio = (float)3/7;
    const float radius = board->getCellWidth() * ratio;

public:
    Piece(const Board * const b, Vector2 p, PieceType t);
    void draw() override;
    float getRadius() const;
};
