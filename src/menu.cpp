#include <sstream>
#include "menu.hpp"
#include "object.hpp"
#include "raylib.h"

Button::Button(Vector2 p, std::string i, int f)
    : Object(p), info(i), fontSize(f) {}

void Button::draw() {
    DrawRectangleV(leftTop, {width, height}, BLACK);
    DrawText(info.c_str(), leftTop.x + width/10, leftTop.y, fontSize, WHITE);
}

bool Button::isToggled(Vector2 point) const {
    return (point.x >= leftTop.x && point.y >= leftTop.y)
        &&
        (point.x <= leftTop.x+width && point.y <= leftTop.y+height);
}


ScoreBoard::ScoreBoard(Vector2 p, const GlobalGame * g)
    : Object(p), globalGame(g) {}
void ScoreBoard::draw() {
    float width = 80, height = 40;
    Vector2 leftTop = Vector2Add(position, {-width/2, -height/2});
    DrawRectangleV(leftTop, {width, height}, RED);

    std::stringstream blackCnt, whiteCnt;
    blackCnt << globalGame->score.first;
    whiteCnt << globalGame->score.second;

    DrawCircle(position.x - width/4, position.y-height/4, 8, BLACK);
    DrawText(blackCnt.str().c_str(),
             position.x - width/4, position.y + height/8,
             16, BLACK);

    DrawCircle(position.x + width/4, position.y-height/4, 8, WHITE);
    DrawText(whiteCnt.str().c_str(),
             position.x + width/4, position.y + height/8,
             16, WHITE);
}
