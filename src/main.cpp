#include "raylib.h"
#include "game.hpp"

int main() {
    const Vector2 screen = {800, 600};

    InitWindow(screen.x, screen.y, "Gomoku By Raylib");
    SetTargetFPS(60);

    Game game(screen);

    while (!game.shouldClose()) {
        game.update();
        game.render();
    }

    CloseWindow();

    return 0;
}
