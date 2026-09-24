#include "raylib.h"
#include "state.hpp"
#include "object.hpp"

int main() {
    GlobalGame globalGame({800, 600}, GameState::INITIAL);

    InitWindow(globalGame.screenLayout.x, globalGame.screenLayout.y, "Gomoku By Raylib");

    LocalGame localGame;
    Board board(&globalGame, &localGame,
                {globalGame.screenLayout.x/2, globalGame.screenLayout.y/2});

    Piece blackPiece(&board, {400, 300}, PieceType::PIECE_BLACK);
    Piece whitePiece(&board, {300, 300}, PieceType::PIECE_WHITE);

    SetTargetFPS(60);


    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(WHITE);

            board.draw();
            blackPiece.draw();
            whitePiece.draw();

            DrawCircleV(GetMousePosition(), blackPiece.getRadius(), RED);
        }
        EndDrawing();
    }

    CloseWindow();
}
