#pragma once

#include <raylib.h>
#include <time.h>
#include "beatings/beatings.h"
#include "pawn/pawn.h"
#include "board/board.h"
#include "beatings/beatings.h"
#include "game/game.h"
#include <cstdlib>
#include <ctime>
#include <climits>
#include <vector>

enum class Level {
    Easy,
    Hard
};

class Ai {
public:
    Level ai_level;
    int number_of_pawns = 0;

    int evaluateBoard(Pawn* board[8][8]);
    int countPawns(Pawn* board[8][8]);
    Pawn* chooseRandomPawn(Pawn* board[8][8], Ai* ai);
    void move(Pawn* pawn, Ai* ai, Board* board);
};