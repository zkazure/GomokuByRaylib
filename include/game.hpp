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
    Button undoButton;
    Button redoButton;
    Button saveGameButton;
    Button loadGameButton;
    Button nextRoundButton;
    ScoreBoard scoreBoard;
    WinLossDeclare winLossDeclare;

    void updateButtons();
    bool handleButtonClicks();
    void handleTurn();
};
