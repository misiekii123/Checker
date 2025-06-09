#pragma once

#include <vector>
#include <raylib.h>
#include "pawn/pawn.h"
#include "board/board.h"

class Beatings {
public:
    Beatings();
    std::vector<std::vector<Vector2>> legalMoves(Pawn* pawn, Board* board);
    bool isInsideBoard(int x, int y);
    std::vector<Vector2> whereIsBeatingAvailable(Pawn* pawn, Board* board);
    std::vector<std::vector<Vector2>> multipleBeatings(Pawn* pawn, Board* board);

private:
    void simulateMultiBeating(Pawn* pawn, std::vector<Vector2> current_path,
                              std::vector<std::vector<Vector2>>& all_paths, Board* board);
    void simulateMultiBeatingInternal(Pawn* tempBoard[8][8], int x, int y,
                                      std::vector<Vector2> current_path,
                                      std::vector<std::vector<Vector2>>& all_paths, Color color);
};
