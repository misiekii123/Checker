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
            if (pawn_selected && selectedPawn) {
                drawPawnSelection(selectedPawn);
                drawAwailableBeating(whereIsBeatingAvailable(selectedPawn));
                drawAwailableMoves(legalMoves(selectedPawn));
            }
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

void Game::drawPawnSelection(Pawn* pawn) {
    if (pawn && pawn->is_alive) {
        Vector2 pos = pawn->getPosition();
        Rectangle selectionRect = { pos.x - 50, pos.y - 50, 100, 100 };
        DrawRectangleLinesEx(selectionRect, 5.0f, YELLOW);
    }
}


void Game::mouseControl() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        int gridX = static_cast<int>(mousePos.x) / 100;
        int gridY = static_cast<int>(mousePos.y) / 100;

        if (gridX >= 0 && gridX < 8 && gridY >= 0 && gridY < 8) {
            Pawn* clickedPawn = board.board[gridY][gridX];

            if (pawn_selected && clickedPawn == selectedPawn) {
                selectedPawn = nullptr;
                pawn_selected = false;
            }
            else if (clickedPawn && clickedPawn->is_alive) {
                selectedPawn = clickedPawn;
                pawn_selected = true;
            }
            else {
                selectedPawn = nullptr;
                pawn_selected = false;
            }
        }
    }
}

std::vector<Vector2> Game::whereIsBeatingAvailable(Pawn* pawn) {
    std::vector<Vector2> result;
    if (!pawn || !pawn->is_alive) return result;

    int directions[4][2] = { {-1, -1}, {1, -1}, {-1, 1}, {1, 1} };
    Vector2 pos = pawn->getPosition();
    int gridX = static_cast<int>(pos.x) / 100;
    int gridY = static_cast<int>(pos.y) / 100;

    if (!pawn->is_queen) {
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

                if (mid && !ColorIsEqual(mid->pawn_color, pawn->pawn_color) && end == nullptr) {
                    result.push_back(Vector2{ static_cast<float>(endX * 100 + 50), static_cast<float>(endY * 100 + 50) });
                }
            }
        }
    } else {
        for (int d = 0; d < 4; ++d) {
            int dx = directions[d][0];
            int dy = directions[d][1];

            int x = gridX + dx;
            int y = gridY + dy;
            bool enemyFound = false;

            while (x >= 0 && x < 8 && y >= 0 && y < 8) {
                Pawn* current = board.board[y][x];

                if (current == nullptr) {
                    if (enemyFound) {
                        result.push_back(Vector2{ static_cast<float>(x * 100 + 50), static_cast<float>(y * 100 + 50) });
                    }
                } else {
                    if (!enemyFound) {
                        if (!ColorIsEqual(current->pawn_color, pawn->pawn_color)) {
                            enemyFound = true;
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

void Game::drawAwailableBeating(std::vector<Vector2> availableBeatings) {
    for (const auto& pos : availableBeatings) {
        Vector2 rect_pos = { pos.x - 50, pos.y - 50 };
        Vector2 rect_size = { 100, 100 };
        DrawRectangleV(rect_pos, rect_size, Color{ 255, 0, 0, 100 });
    }
}

void Game::simulateMultiBeating(Pawn* pawn, std::vector<Vector2> current_path, std::vector<std::vector<Vector2>>& all_paths) {
    Vector2 pos = pawn->getPosition();
    int gridX = static_cast<int>(pos.x) / 100;
    int gridY = static_cast<int>(pos.y) / 100;

    int directions[4][2] = { {-1, -1}, {1, -1}, {-1, 1}, {1, 1} };
    bool beat_found = false;

    for (int d = 0; d < 4; ++d) {
        int dx = directions[d][0];
        int dy = directions[d][1];

        if (!pawn->is_queen) {
            int midX = gridX + dx;
            int midY = gridY + dy;
            int endX = gridX + 2 * dx;
            int endY = gridY + 2 * dy;

            if (midX >= 0 && midX < 8 && midY >= 0 && midY < 8 &&
                endX >= 0 && endX < 8 && endY >= 0 && endY < 8) {

                Pawn* mid = board.board[midY][midX];
                Pawn* end = board.board[endY][endX];

                if (mid && !ColorIsEqual(mid->pawn_color, pawn->pawn_color) && end == nullptr) {
                    beat_found = true;

                    Game clonedGame = *this;
                    Pawn* moved = clonedGame.board.board[gridY][gridX];
                    Pawn* captured = clonedGame.board.board[midY][midX];

                    clonedGame.board.board[gridY][gridX] = nullptr;
                    clonedGame.board.board[midY][midX] = nullptr;
                    captured->is_alive = false;

                    Vector2 newPos = Vector2{ static_cast<float>(endX * 100 + 50), static_cast<float>(endY * 100 + 50) };
                    moved->changePosition(newPos);
                    clonedGame.board.board[endY][endX] = moved;

                    std::vector<Vector2> new_path = current_path;
                    new_path.push_back(newPos);

                    clonedGame.simulateMultiBeating(moved, new_path, all_paths);
                }
            }

        } else {
            int x = gridX + dx;
            int y = gridY + dy;

            bool enemyFound = false;
            int capturedX = -1, capturedY = -1;

            while (x >= 0 && x < 8 && y >= 0 && y < 8) {
                Pawn* current = board.board[y][x];

                if (!enemyFound) {
                    if (current == nullptr) {
                        x += dx;
                        y += dy;
                        continue;
                    }
                    if (!ColorIsEqual(current->pawn_color, pawn->pawn_color)) {
                        enemyFound = true;
                        capturedX = x;
                        capturedY = y;
                        x += dx;
                        y += dy;
                    } else {
                        break;
                    }
                } else {
                    if (current != nullptr) break;

                    beat_found = true;

                    Game clonedGame = *this;
                    Pawn* moved = clonedGame.board.board[gridY][gridX];
                    Pawn* captured = clonedGame.board.board[capturedY][capturedX];

                    clonedGame.board.board[gridY][gridX] = nullptr;
                    clonedGame.board.board[capturedY][capturedX] = nullptr;
                    captured->is_alive = false;

                    Vector2 newPos = Vector2{ static_cast<float>(x * 100 + 50), static_cast<float>(y * 100 + 50) };
                    moved->changePosition(newPos);
                    clonedGame.board.board[y][x] = moved;

                    std::vector<Vector2> new_path = current_path;
                    new_path.push_back(newPos);

                    clonedGame.simulateMultiBeating(moved, new_path, all_paths);

                    x += dx;
                    y += dy;
                }
            }
        }
    }

    if (!beat_found && current_path.size() > 1) {
        all_paths.push_back(current_path);
    }
}


std::vector<std::vector<Vector2>> Game::multipleBeatings(Pawn* pawn) {
    std::vector<std::vector<Vector2>> all_paths;
    Vector2 start = pawn->getPosition();
    std::vector<Vector2> current_path = { start };

    simulateMultiBeating(pawn, current_path, all_paths);

    size_t max_length = 0;
    for (const auto& path : all_paths) {
        if (path.size() > max_length) {
            max_length = path.size();
        }
    }

    std::vector<std::vector<Vector2>> longest_paths;
    for (const auto& path : all_paths) {
        if (path.size() == max_length) {
            longest_paths.push_back(path);
        }
    }

    return longest_paths;
}


std::vector<std::vector<Vector2>> Game::legalMoves(Pawn* pawn) {
    std::vector<std::vector<Vector2>> result;

    if (!pawn || !pawn->is_alive) return result;

    if (!whereIsBeatingAvailable(pawn).empty()) {
        return multipleBeatings(pawn);
    }

    Vector2 pos = pawn->getPosition();
    int gridX = static_cast<int>(pos.x) / 100;
    int gridY = static_cast<int>(pos.y) / 100;

    int directions[4][2] = { {-1, -1}, {1, -1}, {-1, 1}, {1, 1} };

    if (!pawn->is_queen) {
        int dy = ColorIsEqual(pawn->pawn_color, WHITE) ? -1 : 1;

        for (int dx = -1; dx <= 1; dx += 2) {
            int x = gridX + dx;
            int y = gridY + dy;

            if (x >= 0 && x < 8 && y >= 0 && y < 8 && board.board[y][x] == nullptr) {
                Vector2 move = Vector2{ static_cast<float>(x * 100 + 50), static_cast<float>(y * 100 + 50) };
                result.push_back({ pos, move });
            }
        }

    } else {
        for (int d = 0; d < 4; ++d) {
            int dx = directions[d][0];
            int dy = directions[d][1];
            int x = gridX + dx;
            int y = gridY + dy;

            while (x >= 0 && x < 8 && y >= 0 && y < 8) {
                if (board.board[y][x] == nullptr) {
                    Vector2 move = Vector2{ static_cast<float>(x * 100 + 50), static_cast<float>(y * 100 + 50) };
                    result.push_back({ pos, move });
                } else {
                    break;
                }

                x += dx;
                y += dy;
            }
        }
    }

    return result;
}

void Game::drawAwailableMoves(std::vector<std::vector<Vector2>> availableMoves) {
    for (const auto& move : availableMoves) {
        if (move.size() == 2) {
            Vector2 start = move[0];
            Vector2 end = move[1];
            Vector2 rect_pos = { end.x - 50, end.y - 50 };
            Vector2 rect_size = { 100, 100 };
            DrawRectangleV(rect_pos, rect_size, Color{ 0, 255, 0, 100 });
        }
    }
}