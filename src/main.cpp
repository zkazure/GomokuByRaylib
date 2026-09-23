#include <iostream>
#include "raylib.h"

using namespace std;

int main() {
    InitWindow(800, 600, "Gomoku By Raylib");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(WHITE);
        }
        EndDrawing();
    }

    CloseWindow();

    cout << "Hello, Gomoku\n";
}
