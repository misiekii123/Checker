#include "ai.h"
#include "pawn/pawn.h"
#include "board/board.h"
#include "beatings/beatings.h"

Pawn* Ai::chooseRandomPawn(Pawn* board[8][8], Ai* ai) {
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


int Ai::minimax(Pawn* pawn) {
    
}

std::vector<Vector2> Ai::move(Pawn* pawn, Ai* ai) {
    Pawn* board[8][8];
    Beatings* beatings;
    Pawn* pawn_to_move = nullptr;
    if(ai->ai_level == Level::Easy) {
        while(beatings->legalMoves(ai->chooseRandomPawn(board, ai)).empty()) {
            pawn_to_move = chooseRandomPawn(board, ai);
        }
        int randomPath = rand()%beatings->legalMoves(pawn_to_move).size();
        //pawn_to_move->changePosition(beatings->legalMoves(pawn_to_move)[randomPath]]);
    } /*else if(ai->ai_level == Level::Hard) {
        
    }*/
}