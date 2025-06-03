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

            else if (pawn_selected && clickedPawn == nullptr) {
                std::vector<std::vector<Vector2>> availableMoves = legalMoves(selectedPawn);

                for (const auto& move : availableMoves) {
                    if (move.size() == 2) {
                        int moveGridX = static_cast<int>(move[1].x) / 100;
                        int moveGridY = static_cast<int>(move[1].y) / 100;

                        if (moveGridX == gridX && moveGridY == gridY) {
                            Vector2 newPos = move[1];

                            int oldX = static_cast<int>(selectedPawn->getPosition().x) / 100;
                            int oldY = static_cast<int>(selectedPawn->getPosition().y) / 100;

                            board.board[oldY][oldX] = nullptr;
                            board.board[gridY][gridX] = selectedPawn;
                            selectedPawn->changePosition(newPos);

                            selectedPawn = nullptr;
                            pawn_selected = false;
                            break;
                        }
                    }
                }

                for (const auto& move : whereIsBeatingAvailable(selectedPawn)) {
                    int moveGridX = static_cast<int>(move.x) / 100;
                    int moveGridY = static_cast<int>(move.y) / 100;

                    if (moveGridX == gridX && moveGridY == gridY) {
                        Vector2 newPos = move;

                        int oldX = static_cast<int>(selectedPawn->getPosition().x) / 100;
                        int oldY = static_cast<int>(selectedPawn->getPosition().y) / 100;

                        Pawn* midPawn = board.board[(oldY + gridY) / 2][(oldX + gridX) / 2];
                        if (midPawn) {
                            midPawn->is_alive = false; 
                            delete midPawn;
                            board.board[(oldY + gridY) / 2][(oldX + gridX) / 2] = nullptr;
                        }

                        board.board[oldY][oldX] = nullptr;
                        board.board[gridY][gridX] = selectedPawn;
                        selectedPawn->changePosition(newPos);

                        selectedPawn = nullptr;
                        pawn_selected = false;
                        break;
                    }
                }
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
    Pawn* tempBoard[8][8] = {nullptr};

    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            if (board.board[y][x])
                tempBoard[y][x] = new Pawn(*board.board[y][x]);

    int x = pawn->getPosition().x;
    int y = pawn->getPosition().y;

    simulateMultiBeatingInternal(tempBoard, x, y, current_path, all_paths, pawn->pawn_color);

    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            delete tempBoard[y][x];
}

void Game::simulateMultiBeatingInternal(Pawn* tempBoard[8][8], int x, int y,
    std::vector<Vector2> current_path, std::vector<std::vector<Vector2>>& all_paths, Color color)
{
    bool beaten = false;

    const int dirs[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

    for (auto& dir : dirs) {
        int midX = x + dir[0];
        int midY = y + dir[1];
        int destX = x + 2 * dir[0];
        int destY = y + 2 * dir[1];

        if (!isInsideBoard(midX, midY) || !isInsideBoard(destX, destY)) continue;

        Pawn* midPawn = tempBoard[midY][midX];
        if (midPawn && !ColorIsEqual(midPawn->pawn_color, color) && tempBoard[destY][destX] == nullptr) {
            Pawn* movingPawn = tempBoard[y][x];

            tempBoard[y][x] = nullptr;
            tempBoard[midY][midX] = nullptr;
            tempBoard[destY][destX] = movingPawn;

            std::vector<Vector2> new_path = current_path;
            Vector2 newPos;
            newPos.x = destX;
            newPos.y = destY;
            new_path.push_back(newPos);

            simulateMultiBeatingInternal(tempBoard, destX, destY, new_path, all_paths, color);

            tempBoard[destY][destX] = nullptr;
            tempBoard[midY][midX] = midPawn;
            tempBoard[y][x] = movingPawn;

            beaten = true;
        }
    }

    if (!beaten && current_path.size() > 1) {
        all_paths.push_back(current_path);
    }
}


bool Game::isInsideBoard(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
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