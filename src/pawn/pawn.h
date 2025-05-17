#pragma once

#include <raylib.h>

enum class PawnColor {
    White,
    Black
};

class Pawn {
private:
    Vector2 position;
public:
    bool is_alive;
    bool is_queen;
    PawnColor pawn_color;
    Vector2 getPosition();
    void changePosition(Vector2 position);
    Pawn();
};