#include "board.h"

void Board::drawBoard() {
    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            if ((i + j) % 2 == 0) {
                DrawRectangle(i * 100, j * 100, 100, 100, WHITE);
            } else {
                DrawRectangle(i * 100, j * 100, 100, 100, BLACK);
            }
        }
    }
}

Board::Board(Vector2 size) {
    this->size = size;
}