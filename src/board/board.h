#pragma once

#include <raylib.h>
#include "pawn/pawn.h"
#include <map>

class Board {
private:
    Vector2 size;
public:
    Pawn* board[8][8];
    void drawBoard();
    void fillPawns();
    void drawPawns();
    Board(Vector2 size);
    Board() = default;
    ~Board();
};