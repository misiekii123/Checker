#include "pawn.h"

Vector2 Pawn::getPosition() {
    return position;
}

void Pawn::changePosition(Vector2 position) {
    this->position = position;
}

Pawn::Pawn(Color color, Vector2 position) {
    this->position = position;
    this->is_alive = true;
    this->is_queen = false;
    this->pawn_color = color;
}