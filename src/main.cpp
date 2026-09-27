#include <iostream>
#include "raylib.h"
#include "state.hpp"
#include "object.hpp"
#include "type.hpp"

int main() {
    GlobalGame globalGame({800, 600}, GameState::INITIAL);

    InitWindow(globalGame.screenLayout.x, globalGame.screenLayout.y, "Gomoku By Raylib");

    LocalGame localGame;
    Board board(&globalGame, &localGame,
                {globalGame.screenLayout.x/2, globalGame.screenLayout.y/2});

    SetTargetFPS(60);


    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(WHITE);

            board.draw();
        }
        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            std::cout << board.createPiece(GetMousePosition(), PieceType::PIECE_BLACK) << std::endl;
        }
    }

    CloseWindow();
}
