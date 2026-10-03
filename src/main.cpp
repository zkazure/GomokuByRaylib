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

    Button saveGameButton({700, 400}, "SAVE GAME", 14);
    Button loadGameButton({700, 450}, "LOAD GAME", 14);
    Button nextRoundButton({700, 500}, "NEW ROUND", 14);
    ScoreBoard scoreBoard({650, 150}, &globalGame);
    WinLossDeclare winLossDeclare({650, 215}, &localGame);


    SetTargetFPS(60);


    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(UiPalette::window);

            board.draw();

            DrawText("GOMOKU", 650, 75, 24, UiPalette::ink);
            DrawText("FIVE IN A ROW", 651, 100, 10, UiPalette::muted);
            DrawLine(650, 115, 750, 115, UiPalette::ink);

            saveGameButton.draw();
            loadGameButton.draw();

            if (localGame.state != LocalGameState::PLAYING) {
                nextRoundButton.draw();
            }

            scoreBoard.draw();
            winLossDeclare.draw();
            DrawText("CONTROLS", 640, 260, 10, UiPalette::muted);
            DrawRectangleLines(640, 278, 120, 36, UiPalette::ink);
            DrawRectangle(640, 278, 3, 36, UiPalette::accent);
            DrawText("Z", 651, 286, 18, UiPalette::accent);
            DrawText("UNDO", 676, 289, 12, UiPalette::ink);
        }
        EndDrawing();

        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePosition = GetMousePosition();
                if (saveGameButton.isToggled(mousePosition)) {
                    localGame.saveToFile("gomoku.save");
                } else if (loadGameButton.isToggled(mousePosition)) {
                    if (localGame.loadFromFile("gomoku.save")) {
                        board.syncPiecesFromLocalGame();
                        globalGame.state =
                            localGame.state == LocalGameState::PLAYING
                            ? GlobalGameState::PLAYING
                            : GlobalGameState::GAMEOVER;
                    }
                } else if (localGame.state != LocalGameState::PLAYING
                           && nextRoundButton.isToggled(mousePosition)) {
                    globalGame.state = GlobalGameState::PLAYING;
                    board.clear();
                }
            }

            if (IsKeyPressed(KEY_Z)) {
                board.regret();
            }

            if (globalGame.state == GlobalGameState::GAMEOVER) {
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
