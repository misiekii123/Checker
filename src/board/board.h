#pragma once

#include <raylib.h>
#include "pawn/pawn.h"

class Board {
private:
    Vector2 size;
public:
    Pawn black_pawns[12];
    Pawn white_pawns[12];
    void drawBoard();
    void fillPawns();
    void drawPawns();
    Board(Vector2 size);
    Board() = default;
};