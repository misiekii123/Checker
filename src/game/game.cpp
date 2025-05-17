#include "game.h"
#include <math.h>

int max(int table[4]) {
    int max = 0;
    for(int i = 0; i < 4; i++) {
        if(table[i] > max)
            max = table[i];
    }
    return max;
}

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

int Game::isBeatingAvailable(Pawn pawn, std::vector <std::vector<bool>> fields()) {
    int beatings[4];
    Vector2 virtual_position = pawn.getPosition();
    for(int i = 0; i < fields.size(); i++) { // fields.size() is obliged to be changed for the size constant
        beatings[i] = 0;
        if(pawn.is_queen) {
            
        }
        else {
            if(fields[virtual_position.x - 1][virtual_position.y + 1] && !fields[virtual_position.x - 2][virtual_position.y + 2] /*this should include some kind of check of color of the pawn*/) {

            }
        }
    }

    return max(beatings);
}

bool Game::isMoveLegal(Pawn pawn, Vector2 new_position, std::vector <std::vector<bool>> fields()) {
    if(pawn.is_alive){
        if(!fields[new_position.x][new_position.y]) {
            if(pawn.is_queen) {
                if(abs((new_position.y - pawn.getPosition().y)/(new_position.x - pawn.getPosition().x)) == 1) //TODO: ADD checking for pawns on the way
                    return 1;
                else
                    return 0;
            }
            else {
                
            }
        }
    }
}