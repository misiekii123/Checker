#pragma once

#include <raylib.h>
#include "pawn/pawn.h"

class Board {
private:
    Vector2 size;
public:
    Pawn black_pawns[16];
    Pawn white_pawns[16];
    void drawBoard();
    void fillPawns();
    Board(Vector2 size);
    Board() = default;
};