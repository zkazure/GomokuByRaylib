#include "ai.hpp"
#include "game.hpp"

constexpr float AI_THINK_DELAY = 0.35f;

Game::Game(Vector2 screenLayout)
    : globalGame(screenLayout, GlobalGameState::INITIAL),
      localGame(),
      board(&globalGame, &localGame,
            {screenLayout.x / 2, screenLayout.y / 2}),
      undoButton({700, 325}, "UNDO", 14),
      redoButton({700, 370}, "REDO", 14),
      saveGameButton({700, 415}, "SAVE GAME", 14),
      loadGameButton({700, 460}, "LOAD GAME", 14),
      nextRoundButton({700, 505}, "NEW ROUND", 14),
      whiteTypeButton({700, 280}, "WHITE: HUMAN", 12),
      scoreBoard({650, 150}, &globalGame),
      winLossDeclare({650, 215}, &localGame) {}

bool Game::shouldClose() const {
    return WindowShouldClose();
}

void Game::update() {
    updateButtons();

    const bool buttonClicked = handleButtonClicks();

    if (globalGame.state == GlobalGameState::GAMEOVER || buttonClicked) {
        return;
    }

    handleTurn();
}

void Game::render() {
    BeginDrawing();
    ClearBackground(UiPalette::window);

    board.draw();

    DrawText("GOMOKU", 650, 75, 24, UiPalette::ink);

    if (localGame.isCurrentPlayerAI() && localGame.isPlaying()) {
        DrawText("AI THINKING...", 650, 105, 12, UiPalette::muted);
    }

    whiteTypeButton.draw();
    saveGameButton.draw();
    loadGameButton.draw();
    undoButton.draw();
    redoButton.draw();
    nextRoundButton.draw();

    scoreBoard.draw();
    winLossDeclare.draw();

    EndDrawing();
}

void Game::updateButtons() {
    undoButton.setEnabled(localGame.canUndo());
    redoButton.setEnabled(localGame.canRedo());
    nextRoundButton.setEnabled(!localGame.isPlaying());
    whiteTypeButton.setInfo(localGame.isPlayerAI(PieceType::PIECE_WHITE) ? "WHITE: AI"
                                                                       : "WHITE: HUMAN");
}

bool Game::handleButtonClicks() {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return false;
    }

    const Vector2 mousePosition = GetMousePosition();

    if (saveGameButton.isToggled(mousePosition)) {
        localGame.saveToFile("gomoku.save");
        return true;
    }

    if (loadGameButton.isToggled(mousePosition)) {
        if (localGame.loadFromFile("gomoku.save")) {
            board.syncPiecesFromLocalGame();
            globalGame.state = localGame.isPlaying() ? GlobalGameState::PLAYING
                                                     : GlobalGameState::GAMEOVER;
        }
        return true;
    }

    if (undoButton.isToggled(mousePosition)) {
        board.undo();
        while (localGame.isCurrentPlayerAI() && localGame.canUndo()) {
            board.undo();
        }
        return true;
    }

    if (redoButton.isToggled(mousePosition)) {
        board.redo();
        return true;
    }

    if (nextRoundButton.isToggled(mousePosition)) {
        globalGame.state = GlobalGameState::PLAYING;
        board.clear();
        return true;
    }

    if (whiteTypeButton.isToggled(mousePosition)) {
        const PlayerType next = localGame.isPlayerAI(PieceType::PIECE_WHITE)
                              ? PlayerType::HUMAN
                              : PlayerType::AI;
        localGame.setPlayerType(PieceType::PIECE_WHITE, next);
        return true;
    }

    return false;
}

void Game::handleTurn() {
    localGame.checkOutcome();
    if (!localGame.isPlaying()) {
        globalGame.state = GlobalGameState::GAMEOVER;
        if (localGame.getState() == LocalGameState::BLACK_WIN) {
            globalGame.score.first += 1;
        } else {
            globalGame.score.second += 1;
        }
    }

    const bool aiTurn = localGame.isCurrentPlayerAI() && localGame.isPlaying();

    if (!aiTurn && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        board.createPiece(GetMousePosition(), localGame.getCurrentPlaying());
    }

    if (aiTurn) {
        aiWaitTimer += GetFrameTime();
        if (aiWaitTimer >= AI_THINK_DELAY) {
            aiWaitTimer = 0.0f;
            playAIMove();
        }
    } else {
        aiWaitTimer = 0.0f;
    }
}

void Game::playAIMove() {
    const Coordinate move = AI::chooseMove(localGame);
    if (board.createPiece(move, localGame.getCurrentPlaying())) {
        return;
    }

    const int boardSize = localGame.getBoardSize();
    for (int row = 1; row <= boardSize; ++row) {
        for (int col = 1; col <= boardSize; ++col) {
            if (localGame.getPiece(row, col) != PieceType::PIECE_EMPTY) {
                continue;
            }
            if (board.createPiece(Coordinate(row, col), localGame.getCurrentPlaying())) {
                return;
            }
        }
    }
}
