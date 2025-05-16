#include "game.h"

Game::Game() {
    currentState = GameState::InMenu;
    this->board = Board(Vector2{8, 8});
}

void Game::changeGameState(GameState newState) {
    currentState = newState;
}

void Game::startGame() {
    changeGameState(GameState::InGame);
    board.fillPawns();
}

void Game::mainLoop() {
    switch (currentState) {
        case GameState::InGame:
            board.drawBoard();
            board.drawPawns();
            break;
        case GameState::InMenu:
            ui.drawMenu();
            if (IsKeyPressed(KEY_ENTER)) {
                startGame();
            }
            break;
        case GameState::InPause:
            // TODO: Draw UI for the pause menu
            break;
    }
}