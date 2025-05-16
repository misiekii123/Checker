#include "game.h"

Game::Game() {
    currentState = GameState::InMenu;
}

void Game::changeGameState(GameState newState) {
    currentState = newState;
}

void Game::startGame() {
    changeGameState(GameState::InGame);
}

void Game::mainLoop() {
    switch (currentState) {
        case GameState::InGame:
            board.drawBoard();
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