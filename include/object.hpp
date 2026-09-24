#pragma once

#include <algorithm>
#include "raylib.h"

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
    const float ratio = (float)3/4;
    Vector2 layout;
    float width = std::min(layout.x, layout.y);
    int size = 15;

public:
    Board(Vector2 p, Vector2 outterLayout);
    void draw() override;
};
