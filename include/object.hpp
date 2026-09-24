#pragma once

#include <algorithm>
#include "raylib.h"
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
    const LocalGame * localGame;
    const float ratio = (float)3/4;
    float width = ratio * std::min(globalGame->screenLayout.x, globalGame->screenLayout.y);

public:
    Board(const GlobalGame * const globalGame, const LocalGame * localGame, Vector2 p);
    void draw() override;
};

enum class PieceType {PIECE_BLACK, PIECE_WHITE};

class Piece : public Object {
private:
    PieceType type;
    float radius;

public:
    Piece(Vector2 p, PieceType t, float r);
    void draw() override;
};
