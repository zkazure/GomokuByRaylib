#pragma once

#include <string>
#include "object.hpp"
#include "raylib.h"
#include "state.hpp"

class Button : public Object {
private:
    std::string info;
    int fontSize;
    float width = info.length() * 10;
    float height = (float)fontSize;
    Vector2 leftTop = Vector2Add(position, {-width/2, -width/2});

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
