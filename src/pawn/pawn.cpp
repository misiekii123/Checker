#include "pawn.h"

Vector2 Pawn::getPosition() {
    return position;
}

void Pawn::changePosition(Vector2 position) {
    this->position = position;
}

Pawn::Pawn() {
    this->is_alive = true;
}