#include "state.hpp"
#include "object.hpp"

GlobalGame::GlobalGame(Vector2 sl, GameState s)
    : screenLayout(sl), state(s) {}
GlobalGame::GlobalGame(Vector2 sl, GameState s, Color bbg)
    : screenLayout(sl), state(s), boardBackground(bbg) {}

LocalGame::LocalGame() : boardSize(15) {}
LocalGame::LocalGame(int bs) : boardSize(bs) {}
