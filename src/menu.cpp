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
    const Color background = enabled ? UiPalette::panel : UiPalette::disabled;
    const Color foreground = enabled ? UiPalette::ink : UiPalette::disabledInk;
    DrawRectangleRec(bounds, background);
    DrawRectangleLinesEx(bounds, 1, foreground);
    DrawRectangle((int)leftTop.x, (int)leftTop.y, 3, (int)height,
                  enabled ? UiPalette::accent : UiPalette::disabledInk);

    DrawText(info.c_str(), leftTop.x + 14,
             leftTop.y + (height - fontSize)/2, fontSize,
             foreground);
}

bool Button::isToggled(Vector2 point) const {
    return enabled
        && CheckCollisionPointRec(point, {leftTop.x, leftTop.y, width, height});
}

void Button::setEnabled(bool value) {
    enabled = value;
}

ScoreBoard::ScoreBoard(Vector2 p, const LocalGame * lg)
    : Object(p), localGame(lg) {}

void ScoreBoard::draw() {
    DrawText("SCORE", position.x, position.y - 22, 11, UiPalette::muted);

    drawPlayer(PieceType::PIECE_BLACK, position.y + 7);
    drawPlayer(PieceType::PIECE_WHITE, position.y + 37);
}

void ScoreBoard::drawPlayer(PieceType color, float y) const {
    const std::string name = localGame->getName(color);
    const std::string typeText = localGame->isPlayerAI(color) ? "AI" : "HUMAN";
    const std::string label = name + "  " + typeText;
    const std::string scoreText = std::to_string(localGame->getScore(color));
    const float stoneY = y + 8;

    if (color == PieceType::PIECE_BLACK) {
        DrawCircleV({position.x + 6, stoneY}, 6, UiPalette::ink);
    } else {
        DrawCircleV({position.x + 6, stoneY}, 6, UiPalette::panel);
        DrawCircleLines((int)(position.x + 6), (int)stoneY, 6, UiPalette::ink);
    }

    DrawText(label.c_str(), (int)(position.x + 20), (int)y, 12, UiPalette::ink);
    DrawText(scoreText.c_str(),
             (int)(position.x + 140 - MeasureText(scoreText.c_str(), 20)),
             (int)y - 3, 20, UiPalette::ink);
}


WinLossDeclare::WinLossDeclare(Vector2 p, const LocalGame * lg)
    : Object(p), localGame(lg) {}
void WinLossDeclare::draw() {
    std::string declaration;
    if (localGame->getState() == LocalGameState::BLACK_WIN) {
        declaration = localGame->getName(PieceType::PIECE_BLACK) + " wins!";
    } else if (localGame->getState() == LocalGameState::WHITE_WIN) {
        declaration = localGame->getName(PieceType::PIECE_WHITE) + " wins!";
    } else if (localGame->getState() == LocalGameState::DRAW) {
        declaration = "Draw!";
    }

    DrawText(declaration.c_str(), position.x, position.y, 16,
             localGame->isPlaying() ? UiPalette::ink : UiPalette::accent);
}
