#include "game.h"

Game::Game() {
    currentState = GameState::InMenu;
}

void Game::changeGameState(GameState newState) {
    currentState = newState;
}