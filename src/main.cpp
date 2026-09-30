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

    Player blackPlayer(PieceType::PIECE_BLACK);
    Player whitePlayer(PieceType::PIECE_WHITE);
    Player *currPlayer = &blackPlayer;

    Button testButton({700, 500}, "testButton", 16);
    ScoreBoard scoreBoard({700, 100}, &globalGame);


    SetTargetFPS(60);


    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(WHITE);

            board.draw();

            testButton.draw();
            scoreBoard.draw();
        }
        EndDrawing();

        {
            if (globalGame.state == GlobalGameState::GAMEOVER) {
                continue;
            }
        }

        {
            Move lastMove = localGame.getLastMove();
            if (lastMove.type == PieceType::PIECE_BLACK) {
                currPlayer = &whitePlayer;
            } else if (lastMove.type == PieceType::PIECE_WHITE) {
                currPlayer = &blackPlayer;
            }
        }

        {
            localGame.checkOutcome();
            if (localGame.state != LocalGameState::PALYING) {
                globalGame.state = GlobalGameState::GAMEOVER;
                if (localGame.state == LocalGameState::BLACK_WIN) {
                    globalGame.score.first += 1;
                } else {
                    globalGame.score.second += 1;
                }
            }
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            board.createPiece(GetMousePosition(), currPlayer->getPlaying());
            if (testButton.isToggled(GetMousePosition())) {
                globalGame.state = GlobalGameState::GAMEOVER;
            }
        }

        if (IsKeyPressed(KEY_Z)) {
            PieceType type = board.regret();
            if (type == PieceType::PIECE_EMPTY) {
                std::cout << "No History!!!\n";
            } else if (type == PieceType::PIECE_WHITE) {
                currPlayer = &whitePlayer;
            } else {
                currPlayer = &blackPlayer;
            }
        }
    }

    CloseWindow();
}
