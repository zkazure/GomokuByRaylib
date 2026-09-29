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
