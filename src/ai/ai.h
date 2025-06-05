#pragma once

#include <raylib.h>
#include <game/game.h>
#include <vector>

enum class Level {
    Easy,
    Mediium,
    Hard
};

class Ai {
    private:
        Level ai_level;
    public:
        int minimax(Pawn* pawn);
        std::vector<Vector2> move();
};