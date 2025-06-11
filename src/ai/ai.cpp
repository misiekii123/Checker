#include "ai.h"

Ai::Ai() {
    beatings = Beatings();
}

void Ai::move(Board& board) {
    if (ai_level == Level::Easy) {
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                Pawn* pawn = board.board[y][x];
                if (pawn && pawn->is_alive && !ColorIsEqual(pawn->pawn_color, WHITE)) {
                    std::vector<std::vector<Vector2>> moves = beatings.legalMoves(pawn, &board);
                    if (!moves.empty()) {
                        int randomIndex = rand() % moves.size();
                        std::vector<Vector2> selectedMove = moves[randomIndex];
                        Vector2 newPos = selectedMove.back();
                        int oldX = static_cast<int>(pawn->getPosition().x) / 100;
                        int oldY = static_cast<int>(pawn->getPosition().y) / 100;
                        board.board[oldY][oldX] = nullptr;
                        board.board[static_cast<int>(newPos.y) / 100][static_cast<int>(newPos.x) / 100] = pawn;
                        pawn->changePosition(newPos);
                        return; 
                    }
                    else {
                        std::vector<Vector2> availableBeating = beatings.whereIsBeatingAvailable(pawn, &board);
                        if (!availableBeating.empty()) {
                            int randomIndex = rand() % availableBeating.size();
                            Vector2 newPos = availableBeating[randomIndex];
                            int oldX = static_cast<int>(pawn->getPosition().x) / 100;
                            int oldY = static_cast<int>(pawn->getPosition().y) / 100;
                            Pawn* midPawn = board.board[(oldY + static_cast<int>(newPos.y) / 100) / 2][(oldX + static_cast<int>(newPos.x) / 100) / 2];
                            if (midPawn) {
                                midPawn->is_alive = false;
                                board.board[(oldY + static_cast<int>(newPos.y) / 100) / 2][(oldX + static_cast<int>(newPos.x) / 100) / 2] = nullptr;
                            }
                            board.board[oldY][oldX] = nullptr;
                            board.board[static_cast<int>(newPos.y) / 100][static_cast<int>(newPos.x) / 100] = pawn;
                            pawn->changePosition(newPos);
                            return; 
                        }
                    }
                }
            }
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