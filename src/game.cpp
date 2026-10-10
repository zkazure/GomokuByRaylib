#include "ai.hpp"
#include "game.hpp"

constexpr float AI_THINK_DELAY = 0.35f;

Game::Game(Vector2 screenLayout)
    : globalGame(screenLayout, GlobalGameState::INITIAL),
      localGame(),
      board(&globalGame, &localGame,
            {screenLayout.x / 2, screenLayout.y / 2}),
      blackTypeButton({87, 460}, "BLACK: SWITCH", 12),
      whiteTypeButton({87, 505}, "WHITE: SWITCH", 12),
      undoButton({700, 280}, "UNDO", 14),
      redoButton({700, 325}, "REDO", 14),
      saveGameButton({700, 370}, "SAVE GAME", 14),
      loadGameButton({700, 415}, "LOAD GAME", 14),
      nextRoundButton({700, 460}, "NEW ROUND", 14),
      newGameButton({700, 505}, "NEW GAME", 14),
      scoreBoard({650, 150}, &localGame),
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
    ClearBackground(UiPalette::background);

    board.draw();

    DrawText("GOMOKU", 650, 75, 24, UiPalette::foreground);

    blackTypeButton.draw();
    whiteTypeButton.draw();
    saveGameButton.draw();
    loadGameButton.draw();
    undoButton.draw();
    redoButton.draw();
    nextRoundButton.draw();
    newGameButton.draw();

    scoreBoard.draw();
    winLossDeclare.draw();

    EndDrawing();
}

void Game::updateButtons() {
    undoButton.setEnabled(localGame.canUndo());
    redoButton.setEnabled(localGame.canRedo());
    nextRoundButton.setEnabled(!localGame.isPlaying());
}

bool Game::hasHumanPlayer() const {
    return !localGame.isPlayerAI(PieceType::PIECE_BLACK)
        || !localGame.isPlayerAI(PieceType::PIECE_WHITE);
}

void Game::togglePlayerType(PieceType color) {
    const PlayerType next = localGame.isPlayerAI(color) ? PlayerType::HUMAN : PlayerType::AI;
    localGame.setPlayerType(color, next);
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
        while (hasHumanPlayer() && localGame.isCurrentPlayerAI() && localGame.canUndo()) {
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

    if (newGameButton.isToggled(mousePosition)) {
        globalGame.state = GlobalGameState::PLAYING;
        board.clear();
        localGame.resetScores();
        return true;
    }

    if (blackTypeButton.isToggled(mousePosition)) {
        togglePlayerType(PieceType::PIECE_BLACK);
        return true;
    }

    if (whiteTypeButton.isToggled(mousePosition)) {
        togglePlayerType(PieceType::PIECE_WHITE);
        return true;
    }

    return false;
}

void Game::handleTurn() {
    localGame.checkOutcome();
    if (!localGame.isPlaying()) {
        globalGame.state = GlobalGameState::GAMEOVER;
        if (localGame.getState() == LocalGameState::BLACK_WIN) {
            localGame.addScore(PieceType::PIECE_BLACK);
        } else if (localGame.getState() == LocalGameState::WHITE_WIN) {
            localGame.addScore(PieceType::PIECE_WHITE);
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
