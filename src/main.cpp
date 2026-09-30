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

    Button nextRoundButton({700, 500}, "next round", 16);
    ScoreBoard scoreBoard({700, 100}, &globalGame);
    WinLossDeclare winLossDeclare({650, 200}, &localGame);


    SetTargetFPS(60);


    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(WHITE);

            board.draw();

            if (localGame.state != LocalGameState::PLAYING) {
                nextRoundButton.draw();
            }

            scoreBoard.draw();
            winLossDeclare.draw();
        }
        EndDrawing();

        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && nextRoundButton.isToggled(GetMousePosition())) {
                globalGame.state = GlobalGameState::PLAYING;
                board.clear();
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

        if (IsKeyPressed(KEY_Z)) {
            PieceType type = board.regret();
        }
    }

    CloseWindow();
}
