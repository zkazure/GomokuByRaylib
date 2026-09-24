#include "raylib.h"
#include "state.hpp"
#include "object.hpp"

int main() {
    GlobalGame globalGame({800, 600}, GameState::INITIAL);

    InitWindow(globalGame.screenLayout.x, globalGame.screenLayout.y, "Gomoku By Raylib");

    Board board({globalGame.screenLayout.x/2, globalGame.screenLayout.y/2},
                globalGame.screenLayout);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(WHITE);
            board.draw();
        }
        EndDrawing();
    }

    CloseWindow();
}
