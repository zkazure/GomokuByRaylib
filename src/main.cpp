#include <algorithm>
#include "raylib.h"
#include "state.hpp"

class Object {
protected:
    Vector2 position;

public:
    Object() : position({0, 0}) {}
    Object(Vector2 p) : position(p) {}
    virtual void draw() = 0;
};

class Board : public Object {
private:
    const float ratio = (float)3/4;
    Vector2 layout;
    float width = std::min(layout.x, layout.y);
    int size = 15;

public:
    Board(Vector2 p, Vector2 outterLayout)
        : Object(p), layout({outterLayout.x*ratio, outterLayout.y*ratio}) {}
    void draw() override {
        int realSize = size/10 * 600;
        const Vector2 origin = {position.x-width/2, position.y-width/2};
        DrawRectangleLines(origin.x, origin.y,
                           width, width, BLACK);

        float space_width = width / 15;
        for (int i = 1; i < 15; ++i) {
            DrawLine(origin.x + i*space_width, origin.y,
                     origin.x + i*space_width, origin.y + width,
                     BLACK);
            DrawLine(origin.x, origin.y + i*space_width,
                     origin.x + width, origin.y + i*space_width,
                     BLACK);
        }
    }
};

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
