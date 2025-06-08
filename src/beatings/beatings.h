#pragma once

#include <vector>
#include <raylib.h>
#include "pawn/pawn.h"

class Beatings {
public:
    bool isInsideBoard(int x, int y);
    std::vector<Vector2> whereIsBeatingAvailable(Pawn* pawn, Board* board);
    void simulateMultiBeating(Pawn* pawn, std::vector<Vector2> current_path, std::vector<std::vector<Vector2>>& all_paths, Board* board);
    void simulateMultiBeatingInternal(Pawn* tempBoard[8][8], int x, int y, std::vector<Vector2> current_path, std::vector<std::vector<Vector2>>& all_paths, Color color);
    std::vector<std::vector<Vector2>> multipleBeatings(Pawn* pawn);
    std::vector<std::vector<Vector2>> legalMoves(Pawn* pawn, Board* board);
    Beatings();
};