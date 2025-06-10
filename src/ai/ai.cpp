#include "ai.h"

Ai::Ai() {
    this->beatings = Beatings();
}

int Ai::evaluateBoard(Pawn* board[8][8]) {
    int score = 0;

    for(int row = 0; row < 8; ++row) {
        for(int col = 0; col < 8; ++col) {
            Pawn* pawn = board[row][col];
            if (!pawn || !pawn->is_alive) continue;

            bool isBlack = ColorIsEqual(pawn->pawn_color, BLACK);
            int value = pawn->is_queen ? 2 : 1;
            auto moves = beatings.legalMoves(pawn, nullptr);
            int mobility = 0;
            for (const auto& path : moves) {
                mobility += (path.size() - 1);
            }

            int sign = isBlack ? 1 : -1;
            score += sign * (value + 0.25 * mobility);
        }
    }

    return score;
}

int Ai::countPawns(Pawn* board[8][8]) {
    int pawns = 0;
    for(int row = 0; row < 8; ++row) {
        for(int col = 0; col < 8; ++col) {
            Pawn* p = board[row][col];
            if(p && p->is_alive && ColorIsEqual(p->pawn_color, BLACK)) {
                pawns++;
            }
        }
    }
    return pawns;
}

Pawn* Ai::chooseRandomPawn(Pawn* board[8][8]) {
    this->number_of_pawns = countPawns(board);
    if (this->number_of_pawns == 0) return nullptr;
    srand(static_cast<unsigned int>(time(nullptr)));
    int choosedPawn = rand() % this->number_of_pawns;
    int actPawn = 0;

    for (int row = 7; row >= 0; --row) {
        for (int col = 7; col >= 0; --col) {
            Pawn* current = board[row][col];
            if (current && current->is_alive && ColorIsEqual(current->pawn_color, BLACK)) {
                if (actPawn == choosedPawn) {
                    return current;
                }
                actPawn++;
            }
        }
    }

    return nullptr;
}

void Ai::move(Board* board) {
    Pawn* pawn_to_move = nullptr;

    if(this->ai_level == Level::Easy) {
        do {
            pawn_to_move = chooseRandomPawn(board->board);
            if (!pawn_to_move) return;
        } while(beatings.legalMoves(pawn_to_move, board).empty());

        srand(static_cast<unsigned int>(time(nullptr)));
        auto moves = beatings.legalMoves(pawn_to_move, board);
        int randomPath = rand() % moves.size();
        Vector2 movePos = moves[randomPath].back();

        int oldX = static_cast<int>(pawn_to_move->getPosition().x) / 100;
        int oldY = static_cast<int>(pawn_to_move->getPosition().y) / 100;
        int newX = static_cast<int>(movePos.x) / 100;
        int newY = static_cast<int>(movePos.y) / 100;
        board->board[oldY][oldX] = nullptr;
        board->board[newY][newX] = pawn_to_move;
        pawn_to_move->changePosition(movePos);

    } else if(this->ai_level == Level::Hard) {
        int bestScore = INT_MIN;
        Pawn* bestPawn = nullptr;
        std::vector<Vector2> bestMove;
        Pawn* boardCopy[8][8];

        for(int row = 0; row < 8; ++row) {
            for(int col = 0; col < 8; ++col) {
                Pawn* current = board->board[row][col];
                if (current && current->is_alive && ColorIsEqual(current->pawn_color, BLACK)) {
                    auto moves = beatings.legalMoves(current, board);

                    for (const auto& move : moves) {
                        for(int r = 0; r < 8; ++r)
                            for(int c = 0; c < 8; ++c)
                                boardCopy[r][c] = board->board[r][c] ? new Pawn(*board->board[r][c]) : nullptr;

                        int curX = static_cast<int>(current->getPosition().x) / 100;
                        int curY = static_cast<int>(current->getPosition().y) / 100;
                        Pawn* simPawn = boardCopy[curY][curX];

                        Vector2 newPos = move.back();

                        int simOldX = curX;
                        int simOldY = curY;
                        int simNewX = static_cast<int>(newPos.x) / 100;
                        int simNewY = static_cast<int>(newPos.y) / 100;
                        boardCopy[simOldY][simOldX] = nullptr;
                        boardCopy[simNewY][simNewX] = simPawn;
                        simPawn->changePosition(newPos);

                        int score = evaluateBoard(boardCopy);

                        if (score > bestScore) {
                            bestScore = score;
                            bestPawn = current;
                            bestMove = move;
                        }

                        for(int r = 0; r < 8; ++r)
                            for(int c = 0; c < 8; ++c)
                                delete boardCopy[r][c];
                    }
                }
            }
        }

        if (!bestPawn || bestMove.empty()) {
            std::cout << "AI could not find a valid move!" << std::endl;
            return;
        }

        int oldX = static_cast<int>(bestPawn->getPosition().x) / 100;
        int oldY = static_cast<int>(bestPawn->getPosition().y) / 100;
        int newX = static_cast<int>(bestMove.back().x) / 100;
        int newY = static_cast<int>(bestMove.back().y) / 100;

        board->board[oldY][oldX] = nullptr;
        board->board[newY][newX] = bestPawn;
        bestPawn->changePosition(bestMove.back());
    }
}
