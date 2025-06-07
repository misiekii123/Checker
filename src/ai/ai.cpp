#include "ai.h"
#include "pawn/pawn.h"
#include "board/board.h"

Vector2 Ai::chooseRandomPawn(Pawn* board[8][8], Ai* ai) {
    int choosedPawn = rand() % ai->number_of_pawns;
    int actPawn = 0;
    for (int col = 7; col > 0; --col) {
        for (int row = 7; row > 0; --row) {
            if(board[row][col]->is_alive && ColorIsEqual(board[row][col]->pawn_color,BLACK)) {
                if(actPawn == choosedPawn) {
                    return board[row][col]->getPosition();
                }
                else {
                    break;
                }
            }
        }
    }
}

int Ai::minimax(Pawn* pawn) {
    
}

std::vector<Vector2> Ai::move(Pawn* pawn, Ai* ai) {
    /*if(ai->ai_level == Level::Easy) {
        
    }*/
}