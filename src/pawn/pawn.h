#pragma once

#include <raylib.h>

class Pawn {
private:
    Vector2 position;
public:
    bool is_alive;
    bool is_queen;
    Color pawn_color;
    Vector2 getPosition();
    void changePosition(Vector2 position);
    Pawn(Color color, Vector2 position);
};