#include "beatings.h"

Beatings::Beatings() {}

bool Beatings::isInsideBoard(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

std::vector<Vector2> Beatings::whereIsBeatingAvailable(Pawn* pawn, Board* board) {
    std::vector<Vector2> result;
    if (!pawn || !pawn->is_alive || !board) return result;

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

            if (isInsideBoard(midX, midY) && isInsideBoard(endX, endY)) {
                Pawn* mid = board->board[midY][midX];
                Pawn* end = board->board[endY][endX];

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

            while (isInsideBoard(x, y)) {
                Pawn* current = board->board[y][x];

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

void Beatings::simulateMultiBeating(Pawn* pawn, std::vector<Vector2> current_path, std::vector<std::vector<Vector2>>& all_paths, Board* board) {
    Pawn* tempBoard[8][8] = {nullptr};

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (board->board[y][x]) {
                tempBoard[y][x] = new Pawn(*board->board[y][x]);
            }
        }
    }

    int gridX = static_cast<int>(pawn->getPosition().x) / 100;
    int gridY = static_cast<int>(pawn->getPosition().y) / 100;

    simulateMultiBeatingInternal(tempBoard, gridX, gridY, current_path, all_paths, pawn->pawn_color);

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            delete tempBoard[y][x];
        }
    }
}

void Beatings::simulateMultiBeatingInternal(Pawn* tempBoard[8][8], int x, int y,
    std::vector<Vector2> current_path, std::vector<std::vector<Vector2>>& all_paths, Color color) {
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
            Vector2 newPos = { static_cast<float>(destX * 100 + 50), static_cast<float>(destY * 100 + 50) };
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

std::vector<std::vector<Vector2>> Beatings::multipleBeatings(Pawn* pawn, Board* board) {
    std::vector<std::vector<Vector2>> all_paths;
    Vector2 start = pawn->getPosition();
    std::vector<Vector2> current_path = { start };

    simulateMultiBeating(pawn, current_path, all_paths, board);

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

std::vector<std::vector<Vector2>> Beatings::legalMoves(Pawn* pawn, Board* board) {
    std::vector<std::vector<Vector2>> result;

    if (!pawn || !pawn->is_alive || board == nullptr) return result;

    if (!whereIsBeatingAvailable(pawn, board).empty()) {
        return multipleBeatings(pawn, board);
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

            if (isInsideBoard(x, y) && board->board[y][x] == nullptr) {
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

            while (isInsideBoard(x, y)) {
                if (board->board[y][x] == nullptr) {
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
