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
    DrawRectangleRec(bounds, UiPalette::ink);

    int textWidth = MeasureText(info.c_str(), fontSize);
    DrawText(info.c_str(), leftTop.x + (width - textWidth)/2,
             leftTop.y + (height - fontSize)/2, fontSize,
             UiPalette::panel);
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

    DrawText("Score", position.x - 46, position.y - 30, 16, UiPalette::ink);
    DrawCircleV({position.x - 39, position.y + 4}, 7, BLACK);
    DrawText("Black", position.x - 26, position.y - 4, 14, UiPalette::ink);
    DrawText(blackText.c_str(),
             (int)(position.x + 58 - MeasureText(blackText.c_str(), 18)),
             position.y - 5, 18, UiPalette::ink);

    DrawCircleV({position.x - 39, position.y + 32}, 7, WHITE);
    DrawCircleLines((int)position.x - 39, (int)position.y + 32, 7, UiPalette::border);
    DrawText("White", position.x - 26, position.y + 24, 14, UiPalette::ink);
    DrawText(whiteText.c_str(),
             (int)(position.x + 58 - MeasureText(whiteText.c_str(), 18)),
             position.y + 23, 18, UiPalette::ink);
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
             localGame->state == LocalGameState::PLAYING ? UiPalette::ink : GOLD);
}
