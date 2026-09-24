#include "state.hpp"

GlobalGame::GlobalGame(Vector2 s, GameState st)
    : screenLayout(s), state(st) {}


LocalGame::LocalGame() : boardSize(15) {}
LocalGame::LocalGame(int bs) : boardSize(bs) {}
