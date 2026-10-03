#include <sstream>
#include "menu.hpp"
#include "object.hpp"
#include "raylib.h"
#include "state.hpp"
#include "type.hpp"

Button::Button(Vector2 p, std::string i, int f)
    : Object(p), info(i), fontSize(f), width(120), height(40),
      leftTop({p.x - width/2, p.y - height/2}) {}

void Button::draw() {
    Rectangle bounds = {leftTop.x, leftTop.y, width, height};
    DrawRectangleRec(bounds, UiPalette::panel);
    DrawRectangleLinesEx(bounds, 1, UiPalette::ink);
    DrawRectangle((int)leftTop.x, (int)leftTop.y, 3, (int)height,
                  UiPalette::accent);

    DrawText(info.c_str(), leftTop.x + 14,
             leftTop.y + (height - fontSize)/2, fontSize,
             UiPalette::ink);
}

bool Button::isToggled(Vector2 point) const {
    return CheckCollisionPointRec(point, {leftTop.x, leftTop.y, width, height});
}


ScoreBoard::ScoreBoard(Vector2 p, const GlobalGame * gg)
    : Object(p), globalGame(gg) {}
void ScoreBoard::draw() {
    std::stringstream blackCnt, whiteCnt;
    blackCnt << globalGame->score.first;
    whiteCnt << globalGame->score.second;
    const std::string blackText = blackCnt.str();
    const std::string whiteText = whiteCnt.str();

    DrawText("SCORE", position.x, position.y - 22, 11, UiPalette::muted);
    DrawCircleV({position.x + 6, position.y + 15}, 6, UiPalette::ink);
    DrawText("BLACK", position.x + 20, position.y + 7, 12, UiPalette::ink);
    DrawText(blackText.c_str(),
             (int)(position.x + 100 - MeasureText(blackText.c_str(), 20)),
             position.y + 4, 20, UiPalette::ink);

    DrawCircleV({position.x + 6, position.y + 45}, 6, UiPalette::panel);
    DrawCircleLines((int)position.x + 6, (int)position.y + 45, 6, UiPalette::ink);
    DrawText("WHITE", position.x + 20, position.y + 37, 12, UiPalette::ink);
    DrawText(whiteText.c_str(),
             (int)(position.x + 100 - MeasureText(whiteText.c_str(), 20)),
             position.y + 34, 20, UiPalette::ink);
}


WinLossDeclare::WinLossDeclare(Vector2 p, const LocalGame * lg)
    : Object(p), localGame(lg) {}
void WinLossDeclare::draw() {
    const char *declaration = "";
    if (localGame->state == LocalGameState::BLACK_WIN) {
        declaration = "Black wins!";
    } else if (localGame->state == LocalGameState::WHITE_WIN) {
        declaration = "White wins!";
    }

    DrawText(declaration, position.x, position.y, 16,
             localGame->state == LocalGameState::PLAYING ? UiPalette::ink : UiPalette::accent);
}
