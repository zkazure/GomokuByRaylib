#pragma once

#include "raylib.h"
#include "menu.hpp"
#include "object.hpp"
#include "state.hpp"
#include "type.hpp"

class Game {
public:
    Game(Vector2 screenLayout);

    bool shouldClose() const;
    void update();
    void render();

private:
    GlobalGame globalGame;
    LocalGame localGame;
    Board board;
    Button blackTypeButton;
    Button whiteTypeButton;
    Button undoButton;
    Button redoButton;
    Button nextTurnButton;
    Button saveGameButton;
    Button loadGameButton;
    Button nextRoundButton;
    Button newGameButton;
    ScoreBoard scoreBoard;
    WinLossDeclare winLossDeclare;
    float aiWaitTimer = 0.0f;

    void updateButtons();
    bool handleButtonClicks();
    void handleTurn();
    void playAIMove();
    void togglePlayerType(PieceType color);
};
