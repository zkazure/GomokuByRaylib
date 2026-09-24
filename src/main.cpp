#include "raylib.h"
#include "state.hpp"
#include "object.hpp"

int main() {
    GlobalGame globalGame({800, 600}, GameState::INITIAL);

    InitWindow(globalGame.screenLayout.x, globalGame.screenLayout.y, "Gomoku By Raylib");

    LocalGame localGame(15);
    Board board(&globalGame, &localGame,
                {globalGame.screenLayout.x/2, globalGame.screenLayout.y/2});
    Piece blackPiece({400, 300}, PieceType::PIECE_BLACK, 10);
    Piece whitePiece({300, 300}, PieceType::PIECE_WHITE, 10);

    SetTargetFPS(60);


    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(WHITE);

            DrawCircleV(GetMousePosition(), 10, RED);

            board.draw();
            blackPiece.draw();
            whitePiece.draw();

        }
        EndDrawing();
    }

    CloseWindow();
}
