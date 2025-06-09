#include "ai.h"
#include "pawn/pawn.h"
#include "board/board.h"
#include "beatings/beatings.h"

int Ai::evaluateBoard(Pawn* board[8][8]) {
    int score = 0;
    Beatings beatings;

    for(int row = 0; row < 8; ++row) {
        for(int col = 0; col < 8; ++col) {
            Pawn* pawn = board[row][col];
            if (!pawn || !pawn->is_alive) continue;

            bool isBlack = ColorIsEqual(pawn->pawn_color, BLACK);
            int value = pawn->is_queen ? 2 : 1;
            int mobility = beatings.legalMoves(pawn).size();

            int sign = isBlack ? 1 : -1;
            score += sign * (value + 0.25 * mobility);
        }
    }

    return score;
}

int Ai::countPawns(Pawn* board[8][8]) {
    int pawns = 0;
    for(int row = 0; row < 8; ++row) {
        for(int col = 0; col < 0; ++col) {
            if(board[row][col]->is_alive && ColorIsEqual(board[row][col]->pawn_color, BLACK)) {
                pawns++;
            }
        }
    }
    return pawns;
}

Pawn* Ai::chooseRandomPawn(Pawn* board[8][8], Ai* ai) {
    ai->number_of_pawns = countPawns(board);
    srand(time(nullptr));
    int choosedPawn = rand() % ai->number_of_pawns;
    int actPawn = 0;

    for (int col = 7; col >= 0; --col) {
        for (int row = 7; row >= 0; --row) {
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

void Ai::move(Pawn* pawn, Ai* ai) {
    Pawn* board[8][8];
    Beatings* beatings;
    Pawn* pawn_to_move = nullptr;
    if(ai->ai_level == Level::Easy) {
        while(beatings->legalMoves(ai->chooseRandomPawn(board, ai)).empty()) {
            pawn_to_move = chooseRandomPawn(board, ai);
        }
        srand(time(nullptr));
        int randomPath = rand()%beatings->legalMoves(pawn_to_move).size();
        pawn_to_move->changePosition(beatings->legalMoves(pawn_to_move)[randomPath].back());
    } else if(ai->ai_level == Level::Hard) {
        int bestScore = INT_MIN;
        Pawn* bestPawn = nullptr;
        std::vector<Vector2> bestMove;
        Pawn* boardCopy[8][8];
        Beatings beatings;

        for(int row = 0; row < 8; ++row) {
            for(int col = 0; col < 8; ++col) {
                Pawn* current = board[row][col];
                if (current && current->is_alive && ColorIsEqual(current->pawn_color, BLACK)) {
                    std::vector<std::vector<Vector2>> moves = beatings.legalMoves(current);

                    for (const auto& move : moves) {
                        for(int r = 0; r < 8; ++r)
                            for(int c = 0; c < 8; ++c)
                                boardCopy[r][c] = board[r][c] ? new Pawn(*board[r][c]) : nullptr;

                        Pawn* simPawn = boardCopy[int(current->getPosition().y)][int(current->getPosition().x)];

                        simPawn->changePosition(move.back());

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

        if (bestPawn && !bestMove.empty()) {
        bestPawn->changePosition(bestMove.back());
        }
    }
}