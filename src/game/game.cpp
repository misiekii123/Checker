#include "game.h"

Game::Game() {
    currentState = GameState::InMenu;
    this->board = Board(Vector2{8, 8});
    this->beatings = Beatings();
    this->ui = Ui();
    this->ai = Ai();
    ai.ai_level = Level::Easy;
}

void Game::changeGameState(GameState newState) {
    currentState = newState;
}

void Game::startGame() {
    changeGameState(GameState::InGame);
    board.fillPawns();
    playerTurn = Player::Player;
    is_finished = false;
    pawn_selected = false;
    selectedPawn = nullptr;
    winning_color = PINK;
}

void Game::mainLoop() {
    switch (currentState) {
        case GameState::InGame:
            board.drawBoard();
            board.drawPawns();
            if (!pawn_selected) {
                Color currentPlayerColor = (playerTurn == Player::Player) ? player_color : enemy_color;
                if (anyPawnHasBeating(currentPlayerColor)) {
                    Pawn* forced = pawnHasBeatingAvailable(currentPlayerColor);
                    if (forced) {
                        selectedPawn = forced;
                        pawn_selected = true;
                    }
                }
            }
            mouseControl();
            if (is_finished) {
                currentState = GameState::InGameOver;
            }
            if (pawn_selected && selectedPawn) {
                drawPawnSelection(selectedPawn);
                drawAwailableBeating(this->beatings.whereIsBeatingAvailable(selectedPawn, &board));
                drawAwailableMoves(this->beatings.legalMoves(selectedPawn, &board));
            }
            if (playerTurn == Player::Enemy && gameMode == GameMode::SinglePlayer) {
                ai.move(board);
                playerTurn = Player::Player;
            }
            if (isFinished(player_color) || isFinished(enemy_color)) {
                is_finished = true;
                winning_color = isFinished(player_color) ? enemy_color : player_color;
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                changeGameState(GameState::InPause);
            }
            break;
        case GameState::InMenu:
            ui.drawMenu();
            if (IsKeyPressed(KEY_ONE)) {
                startGame();
                gameMode = GameMode::SinglePlayer;
            }
            else if (IsKeyPressed(KEY_TWO)) {
                startGame();
                gameMode = GameMode::MultiPlayer;
            }
            else if (IsKeyPressed(KEY_ESCAPE)) {
                CloseWindow();
            }
            break;
        case GameState::InPause:
            ui.drawPauseMenu();
            if (IsKeyPressed(KEY_ENTER)) {
                currentState = GameState::InGame;
            }
            else if (IsKeyPressed(KEY_ESCAPE)) {
                changeGameState(GameState::InMenu);
            }
            break;
        case GameState::InGameOver:
            ui.drawGameOver(winning_color);
            if (IsKeyPressed(KEY_ESCAPE)) {
                changeGameState(GameState::InMenu);
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
                if (!anyPawnHasBeating(selectedPawn->pawn_color)) {
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
                                if (gameMode == GameMode::MultiPlayer) {
                                    playerTurn = (playerTurn == Player::Player) ? Player::Enemy : Player::Player;
                                }
                                else {
                                    playerTurn = Player::Enemy;
                                }
                                return;
                            }
                        }
                    }
                }

                for (const auto& move : this->beatings.whereIsBeatingAvailable(selectedPawn, &board)) {
                    int moveGridX = static_cast<int>(move.x) / 100;
                    int moveGridY = static_cast<int>(move.y) / 100;
                    if (moveGridX == gridX && moveGridY == gridY) {
                        Vector2 newPos = move;
                        int oldX = static_cast<int>(selectedPawn->getPosition().x) / 100;
                        int oldY = static_cast<int>(selectedPawn->getPosition().y) / 100;

                        int newX = moveGridX;
                        int newY = moveGridY;

                        int dx = (newX - oldX) > 0 ? 1 : -1;
                        int dy = (newY - oldY) > 0 ? 1 : -1;

                        int x = oldX + dx;
                        int y = oldY + dy;

                        while (x != newX && y != newY) {
                            Pawn* midPawn = board.board[y][x];
                            if (midPawn && midPawn->is_alive && !ColorIsEqual(midPawn->pawn_color, selectedPawn->pawn_color)) {
                                winning_color = selectedPawn->pawn_color;
                                is_finished = isFinished(midPawn->pawn_color);
                                midPawn->is_alive = false;
                                board.board[y][x] = nullptr;
                                break;
                            }
                            x += dx;
                            y += dy;
                        }

                        board.board[oldY][oldX] = nullptr;
                        board.board[newY][newX] = selectedPawn;
                        selectedPawn->changePosition(newPos);

                        if (!selectedPawn->is_queen && (newY == 0 || newY == 7)) {
                            selectedPawn->is_queen = true;
                        }

                        if (!beatings.whereIsBeatingAvailable(selectedPawn, &board).empty()) {
                            pawn_selected = true;
                            return;
                        }

                        selectedPawn = nullptr;
                        pawn_selected = false;

                        if (gameMode == GameMode::MultiPlayer) {
                            playerTurn = (playerTurn == Player::Player) ? Player::Enemy : Player::Player;
                        }
                        else {
                            playerTurn = Player::Enemy;
                        }

                        return;
                    }
                }
            }


            else if (clickedPawn && clickedPawn->is_alive) {
                Color currentPlayerColor = (playerTurn == Player::Player) ? player_color : enemy_color;

                if (!ColorIsEqual(clickedPawn->pawn_color, currentPlayerColor)) {
                    selectedPawn = nullptr;
                    pawn_selected = false;
                    return;
                }

                if (anyPawnHasBeating(currentPlayerColor)) {
                    if (!beatings.whereIsBeatingAvailable(clickedPawn, &board).empty()) {
                        selectedPawn = clickedPawn;
                        pawn_selected = true;
                    } else {
                        Pawn* forced = pawnHasBeatingAvailable(currentPlayerColor);
                        if (forced) {
                            selectedPawn = forced;
                            pawn_selected = true;
                        } else {
                            selectedPawn = nullptr;
                            pawn_selected = false;
                        }
                    }
                } else {
                    selectedPawn = clickedPawn;
                    pawn_selected = true;
                }
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

bool Game::isFinished(Color color) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Pawn* pawn = board.board[row][col];
            if (pawn && pawn->is_alive && ColorIsEqual(pawn->pawn_color, color)) {
                return false;
            }
        }
    }
    return true;
}

Pawn* Game::pawnHasBeatingAvailable(Color color) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Pawn* pawn = board.board[row][col];
            if (pawn && pawn->is_alive && ColorIsEqual(pawn->pawn_color, color)) {
                if (!beatings.whereIsBeatingAvailable(pawn, &board).empty()) {
                    return pawn;
                }
            }
        }
    }
    return nullptr;
}

bool Game::anyPawnHasBeating(Color color) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Pawn* pawn = board.board[row][col];
            if (pawn && pawn->is_alive && ColorIsEqual(pawn->pawn_color, color)) {
                if (!beatings.whereIsBeatingAvailable(pawn, &board).empty()) {
                    return true;
                }
            }
        }
    }
    return false;
}