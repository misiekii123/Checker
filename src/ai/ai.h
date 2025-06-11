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
public:
    void move(Board& board);
    Ai();
};