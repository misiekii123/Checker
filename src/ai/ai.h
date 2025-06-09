#pragma once

#include <raylib.h>
#include <game/game.h>
#include <vector>
#include "beatings/beatings.h"

enum class Level {
    Easy,
    Hard
};

class Ai {
    private:
        Level ai_level;
        int number_of_pawns = 12;
    public:
        void copyBoard(Pawn* src[8][8], Pawn* dest[8][8]);
        int evaluateBoard(Pawn* board[8][8]);
        Pawn* chooseRandomPawn(Pawn* board[8][8], Ai* ai);
        int minimax(Pawn* pawn, Pawn* board[8][8]);
        void move(Pawn* pawn, Ai* ai);
};