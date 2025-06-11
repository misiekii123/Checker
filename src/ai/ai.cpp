#include "ai.h"

Ai::Ai() {
    beatings = Beatings();
    srand(std::time(nullptr));
}

void Ai::move(Board& board) {
    if (ai_level == Level::Easy) {
        std::vector<std::tuple<Pawn*, std::vector<Vector2>>> beatingsList;
        std::vector<std::tuple<Pawn*, std::vector<Vector2>>> normalMovesList;

        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                Pawn* pawn = board.board[y][x];
                if (pawn && pawn->is_alive && !ColorIsEqual(pawn->pawn_color, WHITE)) {
                    std::vector<Vector2> availableBeatings = beatings.whereIsBeatingAvailable(pawn, &board);
                    for (const auto& beat : availableBeatings) {
                        beatingsList.push_back({ pawn, { beat } });
                    }

                    std::vector<std::vector<Vector2>> moves = beatings.legalMoves(pawn, &board);
                    for (const auto& move : moves) {
                        if (move.size() == 2) {
                            normalMovesList.push_back({ pawn, move });
                        }
                    }
                }
            }
        }

        if (!beatingsList.empty()) {
            auto [pawn, moveVec] = beatingsList[rand() % beatingsList.size()];
            Vector2 newPos = moveVec[0];
            int oldX = static_cast<int>(pawn->getPosition().x) / 100;
            int oldY = static_cast<int>(pawn->getPosition().y) / 100;
            int newX = static_cast<int>(newPos.x) / 100;
            int newY = static_cast<int>(newPos.y) / 100;

            int dx = (newX - oldX) > 0 ? 1 : -1;
            int dy = (newY - oldY) > 0 ? 1 : -1;

            int x = oldX + dx;
            int y = oldY + dy;

            while (x != newX && y != newY) {
                Pawn* midPawn = board.board[y][x];
                if (midPawn && midPawn->is_alive && !ColorIsEqual(midPawn->pawn_color, pawn->pawn_color)) {
                    midPawn->is_alive = false;
                    board.board[y][x] = nullptr;
                    break;
                }
                x += dx;
                y += dy;
            }

            board.board[oldY][oldX] = nullptr;
            board.board[newY][newX] = pawn;
            pawn->changePosition(newPos);
            return;
        }
        else if (!normalMovesList.empty()) {
            auto [pawn, moveVec] = normalMovesList[rand() % normalMovesList.size()];
            Vector2 newPos = moveVec[1];
            int oldX = static_cast<int>(pawn->getPosition().x) / 100;
            int oldY = static_cast<int>(pawn->getPosition().y) / 100;
            int newX = static_cast<int>(newPos.x) / 100;
            int newY = static_cast<int>(newPos.y) / 100;

            board.board[oldY][oldX] = nullptr;
            board.board[newY][newX] = pawn;
            pawn->changePosition(newPos);
            return;
        }
    }

    else if (ai_level == Level::Hard) {
        int bestScore = INT_MIN;
        std::vector<Vector2> bestMove;

        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                Pawn* pawn = board.board[y][x];
                if (pawn && pawn->is_alive && !ColorIsEqual(pawn->pawn_color, WHITE)) {
                    std::vector<std::vector<Vector2>> moves = beatings.legalMoves(pawn, &board);
                    for (const auto& move : moves) {
                        Vector2 newPos = move.back();
                        int oldX = static_cast<int>(pawn->getPosition().x) / 100;
                        int oldY = static_cast<int>(pawn->getPosition().y) / 100;
                        board.board[oldY][oldX] = nullptr;
                        board.board[static_cast<int>(newPos.y) / 100][static_cast<int>(newPos.x) / 100] = pawn;
                        pawn->changePosition(newPos);

                        int score = evaluateBoard(board);
                        if (score > bestScore) {
                            bestScore = score;
                            bestMove = move;
                        }

                        board.board[static_cast<int>(newPos.y) / 100][static_cast<int>(newPos.x) / 100] = nullptr;
                        board.board[oldY][oldX] = pawn;
                        pawn->changePosition(Vector2{ static_cast<float>(oldX * 100 + 50), static_cast<float>(oldY * 100 + 50) });
                    }
                }
            }
        }

        if (!bestMove.empty()) {
            Vector2 newPos = bestMove.back();
            int oldX = static_cast<int>(bestMove.front().x) / 100;
            int oldY = static_cast<int>(bestMove.front().y) / 100;
            board.board[oldY][oldX] = nullptr;
            board.board[static_cast<int>(newPos.y) / 100][static_cast<int>(newPos.x) / 100] = board.board[oldY][oldX];
            board.board[static_cast<int>(newPos.y) / 100][static_cast<int>(newPos.x) / 100]->changePosition(newPos);
        }
    }
}

int Ai::evaluateBoard(const Board& board) {
    int score = 0;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Pawn* pawn = board.board[y][x];
            if (pawn && pawn->is_alive) {
                if (ColorIsEqual(pawn->pawn_color, WHITE)) {
                    score += pawn->is_queen ? 5 : 1;
                } else {
                    score -= pawn->is_queen ? 5 : 1;
                }
            }
        }
    }
    return score;
}