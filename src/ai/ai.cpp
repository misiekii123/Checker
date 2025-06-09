#include "ai.h"
#include "pawn/pawn.h"
#include "board/board.h"
#include "beatings/beatings.h"

void Ai::copyBoard(Pawn* src[8][8], Pawn* dest[8][8]) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (src[row][col]) {
                dest[row][col] = new Pawn(*src[row][col]);
            } else {
                dest[row][col] = nullptr;
            }
        }
    }
}

Pawn* Ai::chooseRandomPawn(Pawn* board[8][8], Ai* ai) {
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

int Ai::evaluateBoard(Pawn* board[8][8]) {
    int score = 0;
    Beatings beatings;

    for(int row = 0; row < 8; ++row) {
        for(int col = 0; col < 8; ++col) {
            if(board[row][col]->is_alive && !ColorIsEqual(board[row][col]->pawn_color, BLACK) && !board[row][col]->is_queen) {
                score -= 1;
                score -= 0.25 * beatings.legalMoves(board[row][col]).size();
            } else if(board[row][col]->is_alive && !ColorIsEqual(board[row][col]->pawn_color, BLACK) && board[row][col]->is_queen) {
                score -= 2;
                score -= 0.25 * beatings.legalMoves(board[row][col]).size();
            } else if(board[row][col]->is_alive && ColorIsEqual(board[row][col]->pawn_color, BLACK) && !board[row][col]->is_queen) {
                score += 1;
                score += 0.25 * beatings.legalMoves(board[row][col]).size();
            } else if(board[row][col]->is_alive && ColorIsEqual(board[row][col]->pawn_color, BLACK) && board[row][col]->is_queen) {
                score += 2;
                score += 0.25 * beatings.legalMoves(board[row][col]).size();
            }
        }
    }

    return score;
}


int Ai::minimax(Pawn* pawn, Pawn* board[8][8]) {
    
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
    } else if(ai->ai_level == Level::Hard) { //TODO
        /*if (bestPawn && !bestMove.empty()) {
            bestPawn->changePosition(bestMove.back());
        }*/
    }
}