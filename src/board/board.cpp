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

void Board::fillPawns() {
    for (int i = 0; i < 16; i++) {
        black_pawns[i].changePosition(Vector2{(float)(i % 8) * 100 + 50, (float)(i / 8) * 100 + 50});
        white_pawns[i].changePosition(Vector2{(float)(i % 8) * 100 + 50, (float)(i / 8) * 100 + 50});
    }
}

Board::Board(Vector2 size) {
    this->size = size;
}