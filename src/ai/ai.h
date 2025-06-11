#pragma once

#include <raylib.h>
#include <time.h>
#include "beatings/beatings.h"
#include "pawn/pawn.h"
#include "board/board.h"
#include <cstdlib>
#include <ctime>
#include <climits>
#include <vector>
#include <iostream>

enum class Level {
    Easy,
    Hard
};

class Ai {
private:
    Beatings beatings;
    int evaluateBoard(const Board& board);
public:
    Level ai_level;
    void move(Board& board);
    Ai();
};