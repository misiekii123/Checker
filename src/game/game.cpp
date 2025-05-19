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

std::vector<Vector2> Game::isBeatingAvailable(Pawn* pawn) {
    std::vector<Vector2> result;
    if (!pawn || !pawn->is_alive) return result;

    int directions[4][2] = { {-1, -1}, {1, -1}, {-1, 1}, {1, 1} };
    Vector2 pos = pawn->getPosition();
    int gridX = static_cast<int>(pos.x) / 100;
    int gridY = static_cast<int>(pos.y) / 100;

    for (int d = 0; d < 4; ++d) {
        int dx = directions[d][0];
        int dy = directions[d][1];
        int midX = gridX + dx;
        int midY = gridY + dy;
        int endX = gridX + 2 * dx;
        int endY = gridY + 2 * dy;

        if (midX >= 0 && midX < 8 && midY >= 0 && midY < 8 &&
            endX >= 0 && endX < 8 && endY >= 0 && endY < 8) {

            Pawn* mid = board.board[midY][midX];
            Pawn* end = board.board[endY][endX];

            if (mid && !colorsEqual(mid->pawn_color, pawn->pawn_color) && end == nullptr) {
                result.push_back(Vector2{ static_cast<float>(endX * 100 + 50), static_cast<float>(endY * 100 + 50) });
            }
        }
    }

    return result;
}


// bool Game::isMoveLegal(Pawn pawn, Vector2 new_position, std::vector <std::vector<bool>> fields()) {
//     if(pawn.is_alive){
//         if(!fields[new_position.x][new_position.y]) {
//             if(pawn.is_queen) {
//                 if(abs((new_position.y - pawn.getPosition().y)/(new_position.x - pawn.getPosition().x)) == 1) //TODO: ADD checking for pawns on the way
//                     return 1;
//                 else
//                     return 0;
//             }
//             else {
                
//             }
//         }
//     }
// }


bool Game::colorsEqual(Color c1, Color c2) {
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}