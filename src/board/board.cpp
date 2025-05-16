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
    int black_index = 0;
    int white_index = 0;

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 1 && black_index < 12) {
                float x = col * 100 + 50;
                float y = row * 100 + 50;
                black_pawns[black_index].changePosition(Vector2{x, y});
            }
        }
    }

    for (int row = 5; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 1 && white_index < 12) {
                float x = col * 100 + 50;
                float y = row * 100 + 50;
                white_pawns[white_index].changePosition(Vector2{x, y});
            }
        }
    }
}

Board::Board(Vector2 size) {
    this->size = size;
}