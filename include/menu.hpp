#pragma once

#include <string>
#include "object.hpp"
#include "raylib.h"
#include "state.hpp"

class Button : public Object {
private:
    std::string info;
    int fontSize;
    float width;
    float height;
    Vector2 leftTop;

public:
    Button(Vector2 p, std::string i, int f);
    void draw() override;
    bool isToggled(Vector2 point) const;
};


class ScoreBoard : public Object {
private:
    const GlobalGame * globalGame;

public:
    ScoreBoard(Vector2 p, const GlobalGame * gg);
    void draw() override;
};


class WinLossDeclare : public Object {
private:
    const LocalGame * localGame;

public:
    WinLossDeclare(Vector2 p, const LocalGame * lg);
    void draw() override;
};
