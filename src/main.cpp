#include "raylib.h"

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
    int size = 15;

public:
    Board() {}
    Board(Vector2 p) : Object(p) {}
    void draw() override {
        int realSize = size * 10;
        DrawRectangle(position.x - (float)realSize/2, position.y - (float)realSize/2,
                      realSize, realSize, RED);
    }
};

int main() {
    const int screenWidth = 800, screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Gomoku By Raylib");

    Board board({(float)screenWidth/2, (float)screenHeight/2});

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
