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
        Pawn* chooseRandomPawn(Pawn* board[8][8], Ai* ai);
        int minimax(Pawn* pawn);
        std::vector<Vector2> move(Pawn* pawn, Ai* ai);
};