#include "game.h"
#include <math.h>
#include <iostream>
#include "beatings/beatings.h"

Game::Game() {
    currentState = GameState::InMenu;
    this->board = Board(Vector2{8, 8});
    this->beatings = Beatings();
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
                drawAwailableBeating(this->beatings.whereIsBeatingAvailable(selectedPawn, &board));
                drawAwailableMoves(this->beatings.legalMoves(selectedPawn, &board));
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
                std::vector<std::vector<Vector2>> availableMoves = this->beatings.legalMoves(selectedPawn, &board);
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
                            if (!selectedPawn->is_queen && (gridY == 0 || gridY == 7)) {
                                selectedPawn->is_queen = true;
                            }
                            selectedPawn = nullptr;
                            pawn_selected = false;
                            return;
                        }
                    }
                }

                if (selectedPawn) {
                    for (const auto& move : this->beatings.whereIsBeatingAvailable(selectedPawn, &board)) {
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
                            if (!selectedPawn->is_queen && (gridY == 0 || gridY == 7)) {
                                selectedPawn->is_queen = true;
                            }
                            selectedPawn = nullptr;
                            pawn_selected = false;
                            return;
                        }
                    }
                }
            }

            else if (clickedPawn && clickedPawn->is_alive && ColorIsEqual(clickedPawn->pawn_color, player_color)) {
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

void Game::drawAwailableBeating(std::vector<Vector2> availableBeatings) {
    for (const auto& pos : availableBeatings) {
        Vector2 rect_pos = { pos.x - 50, pos.y - 50 };
        Vector2 rect_size = { 100, 100 };
        DrawRectangleV(rect_pos, rect_size, Color{ 255, 0, 0, 100 });
    }
}

void Game::drawAwailableMoves(std::vector<std::vector<Vector2>> availableMoves) {
    for (const auto& move : availableMoves) {
        if (move.size() == 2) {
            Vector2 end = move[1];
            Vector2 rect_pos = { end.x - 50, end.y - 50 };
            Vector2 rect_size = { 100, 100 };
            DrawRectangleV(rect_pos, rect_size, Color{ 0, 255, 0, 100 });
        }
    }
}
