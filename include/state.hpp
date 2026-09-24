#pragma once
#include "raylib.h"

enum class GameState { INITIAL, PLAYING, GAMEOVER };

class GlobalGame {
public:
    Vector2 screenLayout = {800, 600};
    GameState state = GameState::INITIAL;

public:
    GlobalGame(Vector2 s, GameState st);
};
