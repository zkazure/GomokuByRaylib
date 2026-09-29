#include <iostream>
#include "menu.hpp"
#include "raylib.h"
#include "state.hpp"
#include "object.hpp"
#include "type.hpp"
#include "player.hpp"

int main() {
    GlobalGame globalGame({800, 600}, GameState::INITIAL);

    InitWindow(globalGame.screenLayout.x, globalGame.screenLayout.y, "Gomoku By Raylib");

    LocalGame localGame;
    Board board(&globalGame, &localGame,
                {globalGame.screenLayout.x/2, globalGame.screenLayout.y/2});

    Player blackPlayer(PieceType::PIECE_BLACK);
    Player whitePlayer(PieceType::PIECE_WHITE);
    Player *currPlayer = &blackPlayer;

    Button testButton({400, 300}, "testButton", 16);


    SetTargetFPS(60);


    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(WHITE);

            board.draw();

            testButton.draw();
        }
        EndDrawing();

        {
            Move lastMove = localGame.getLastMove();
            if (lastMove.type == PieceType::PIECE_BLACK) {
                currPlayer = &whitePlayer;
            } else if (lastMove.type == PieceType::PIECE_WHITE) {
                currPlayer = &blackPlayer;
            }
        }

        {
            PieceType type = localGame.checkOutcome();
            if (type != PieceType::PIECE_EMPTY) {
                if (type == PieceType::PIECE_BLACK) {
                    std::cout << "Black win!!!\n";
                } else if (type == PieceType::PIECE_WHITE) {
                    std::cout << "White win!!!\n";
                }
            }
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            board.createPiece(GetMousePosition(), currPlayer->getPlaying());
            testButton.isToggled(GetMousePosition());
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
