#pragma once

#include <vector>
#include <algorithm>
#include "raylib.h"
#include "raymath.h"
#include "state.hpp"
#include "type.hpp"

class Object {
protected:
    Vector2 position;

public:
    Object();
    Object(Vector2 p);
    virtual void draw() = 0;
};

class Piece;

class Board : public Object {
private:
    GlobalGame * const globalGame;
    LocalGame *localGame;
    const float ratio = (float)3/4;
    const float width = ratio * std::min(globalGame->screenLayout.x, globalGame->screenLayout.y);
    const float cellWidth = width / (localGame->boardSize + 1);
    const Vector2 leftTop = Vector2Add(position, {-width/2, -width/2});

    std::vector<std::vector<Vector2>> intersections;
    std::vector<std::vector<Piece *>> pieces;

public:
    Board(GlobalGame * const globalGame, LocalGame * localGame, Vector2 p);
    ~Board();
    void draw() override;
    float getWidth() const ;
    float getCellWidth() const;
    Coordinate pos2coor(Vector2 position) const;
    Vector2 coor2pos(Coordinate coor) const;
    bool createPiece(Coordinate coor, PieceType type);
    bool createPiece(Vector2 position, PieceType type);
    PieceType undo();
    PieceType redo();
    void clear();
    void syncPiecesFromLocalGame();
};


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
    PieceType getType() const;
};
