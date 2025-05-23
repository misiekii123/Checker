#include "game.h"
#include <math.h>
#include <iostream>

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
            mouseControl();
            if (IsKeyPressed(KEY_ESCAPE)) {
                changeGameState(GameState::InPause);
            }
            break;
        case GameState::InMenu:
            ui.drawMenu();
            if (IsKeyPressed(KEY_ENTER)) {
                startGame();
            }
            break;
        case GameState::InPause:
            ui.drawPauseMenu();
            if (IsKeyPressed(KEY_ENTER)) {
                startGame();
            }
            break;
    }
}

bool Game::isPawnSelected(Pawn* pawn) {
    if (pawn) {
        Vector2 pos = pawn->getPosition();
        int gridX = static_cast<int>(pos.x) / 100;
        int gridY = static_cast<int>(pos.y) / 100;
        std::cout << "gridX: " << gridX << " gridY: " << gridY << std::endl;
        return board.board[gridY][gridX] == pawn;
    }
    return false;
}

void Game::mouseControl() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        int gridX = static_cast<int>(mousePos.x) / 100;
        int gridY = static_cast<int>(mousePos.y) / 100;

        std::cout << "gridX: " << gridX << " gridY: " << gridY << std::endl;

        if (gridX >= 0 && gridX < 8 && gridY >= 0 && gridY < 8) {
                  
            if (pawn_selected) {
                // TODO: add logic for moving the pawn
            }
            else {
                Pawn* selectedPawn = board.board[gridY][gridX];
                this->pawn_selected = isPawnSelected(selectedPawn);
                pawn_selected = false;
            }
        }
    }
}

std::vector<Vector2> Game::whereIsBeatingAvailable(Pawn* pawn) {
    std::vector<Vector2> result;
    if (!pawn || !pawn->is_alive) return result;

    if(!pawn->is_queen) {
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
    }
    else {
        int directions[4][2] = { {-1, -1}, {1, -1}, {-1, 1}, {1, 1} };
        Vector2 pos = pawn->getPosition();
        int gridX = static_cast<int>(pos.x) / 100;
        int gridY = static_cast<int>(pos.y) / 100;

        for (int d = 0; d < 4; ++d) {
            int dx = directions[d][0];
            int dy = directions[d][1];
            
            int x = gridX + dx;
            int y = gridY + dy;
            bool enemyFound = false;
            Pawn* captured = nullptr;

            while (x >= 0 && x < 8 && y >= 0 && y < 8) {
                Pawn* current = board.board[y][x];

                if (current == nullptr) {
                    if (enemyFound) {
                        result.push_back(Vector2{ static_cast<float>(x * 100 + 50), static_cast<float>(y * 100 + 50) });
                    }
                } else {
                    if (!enemyFound) {
                        if (!colorsEqual(current->pawn_color, pawn->pawn_color)) {
                            enemyFound = true;
                            captured = current;
                        } else {
                            break;
                        }
                    } else {
                        break;
                    }
                }

                x += dx;
                y += dy;
            }
        }

    }

    return result;
}


// std::vector<Vector2> Game::legalMoves(Pawn* pawn) {
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