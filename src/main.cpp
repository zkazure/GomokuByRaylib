#include <iostream>
#include "raylib.h"

using namespace std;

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
    Board(Vector2 p, int s) : Object(p), size(s) {}
    void draw() override {
        DrawRectangle(position.x, position.y, size, size, RED);
    }
};

int main() {
    InitWindow(800, 600, "Gomoku By Raylib");

    Board board;

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

    cout << "Hello, Gomoku\n";
}
