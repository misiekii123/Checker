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
        int countPawns(Pawn* board[8][8]);
        void freeBoard(Pawn** board[8][8]);
        Pawn** copyBoard(Pawn* src[8][8]);
        int evaluateBoard(Pawn* board[8][8]);
        Pawn* chooseRandomPawn(Pawn* board[8][8], Ai* ai);
        int minimax(Pawn* board[8][8], int depth, bool maximizingPlayer);
        void move(Pawn* pawn, Ai* ai);
        std::vector<std::pair<Pawn**, std::pair<int, int>>> generateMoves(Pawn* board[8][8], Color playerColor);
};