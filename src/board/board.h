#pragma once

#include <raylib.h>
#include "pawn/pawn.h"
#include <vector>
#include <map>

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
    std::vector <std::vector<bool>> fields(); // type of this table should be changed
};