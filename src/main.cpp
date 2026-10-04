#include <iostream>
#include "menu.hpp"
#include "raylib.h"
#include "state.hpp"
#include "object.hpp"
#include "type.hpp"
#include "player.hpp"

int main() {
    GlobalGame globalGame({800, 600}, GlobalGameState::INITIAL);

    InitWindow(globalGame.screenLayout.x, globalGame.screenLayout.y, "Gomoku By Raylib");

    LocalGame localGame;
    Board board(&globalGame, &localGame,
                {globalGame.screenLayout.x/2, globalGame.screenLayout.y/2});

    Player *currPlayer = &localGame.player;

    Button undoButton({700, 355}, "UNDO", 14);
    Button redoButton({700, 395}, "REDO", 14);
    Button saveGameButton({700, 435}, "SAVE GAME", 14);
    Button loadGameButton({700, 475}, "LOAD GAME", 14);
    Button nextRoundButton({700, 505}, "NEW ROUND", 14);
    ScoreBoard scoreBoard({650, 150}, &globalGame);
    WinLossDeclare winLossDeclare({650, 215}, &localGame);


    SetTargetFPS(60);


    while (!WindowShouldClose()) {
        undoButton.setEnabled(!localGame.moveHistory.empty());
        redoButton.setEnabled(!localGame.rmoveHistory.empty());
        nextRoundButton.setEnabled(localGame.state != LocalGameState::PLAYING);

        BeginDrawing();
        {
            ClearBackground(UiPalette::window);

            board.draw();

            DrawText("GOMOKU", 650, 75, 24, UiPalette::ink);
            DrawText("FIVE IN A ROW", 651, 100, 10, UiPalette::muted);
            DrawLine(650, 115, 750, 115, UiPalette::ink);

            saveGameButton.draw();
            loadGameButton.draw();
            undoButton.draw();
            redoButton.draw();

            nextRoundButton.draw();

            scoreBoard.draw();
            winLossDeclare.draw();
        }
        EndDrawing();

        {
            bool buttonClicked = false;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePosition = GetMousePosition();
                if (saveGameButton.isToggled(mousePosition)) {
                    buttonClicked = true;
                    localGame.saveToFile("gomoku.save");
                } else if (loadGameButton.isToggled(mousePosition)) {
                    buttonClicked = true;
                    if (localGame.loadFromFile("gomoku.save")) {
                        board.syncPiecesFromLocalGame();
                        globalGame.state =
                            localGame.state == LocalGameState::PLAYING
                            ? GlobalGameState::PLAYING
                            : GlobalGameState::GAMEOVER;
                    }
                } else if (undoButton.isToggled(mousePosition)) {
                    buttonClicked = true;
                    board.undo();
                } else if (redoButton.isToggled(mousePosition)) {
                    buttonClicked = true;
                    board.redo();
                } else if (nextRoundButton.isToggled(mousePosition)) {
                    buttonClicked = true;
                    globalGame.state = GlobalGameState::PLAYING;
                    board.clear();
                }
            }

            if (globalGame.state == GlobalGameState::GAMEOVER) {
                continue;
            }

            if (buttonClicked) {
                continue;
            }
        }

        {
            localGame.checkOutcome();
            if (localGame.state != LocalGameState::PLAYING) {
                globalGame.state = GlobalGameState::GAMEOVER;
                if (localGame.state == LocalGameState::BLACK_WIN) {
                    globalGame.score.first += 1;
                } else {
                    globalGame.score.second += 1;
                }
            }
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            board.createPiece(GetMousePosition(), currPlayer->playing);
        }

    }

    CloseWindow();
}
