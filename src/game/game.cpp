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
            //TODO: Implement game logic
            break;
        case GameState::InMenu:
            //TODO: Draw UI for the menu
            break;
        case GameState::InPause:
            // TODO: Draw UI for the pause menu
            break;
    }
}