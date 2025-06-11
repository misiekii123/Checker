#include "ai.h"

Ai::Ai() {
    beatings = Beatings();
}

void Ai::move(Board& board) {
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
            }
        }
    }
}