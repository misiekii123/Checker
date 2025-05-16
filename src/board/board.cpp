#include "board.h"

void Board::drawBoard() {
    for (int i = 0; i < size.x; i++) {
        for (int j = 0; j < size.y; j++) {
            if ((i + j) % 2 == 0) {
                DrawRectangle(i * 100, j * 100, 100, 100, GRAY);
            } else {
                DrawRectangle(i * 100, j * 100, 100, 100, DARKGRAY);
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

void Board::drawPawns() {
    for (int i = 0; i < 16; i++) {
        if (black_pawns[i].getPosition().x != 0 && black_pawns[i].getPosition().y != 0) {
            DrawCircle(black_pawns[i].getPosition().x, black_pawns[i].getPosition().y, 40, BLACK);
        }
        if (white_pawns[i].getPosition().x != 0 && white_pawns[i].getPosition().y != 0) {
            DrawCircle(white_pawns[i].getPosition().x, white_pawns[i].getPosition().y, 40, WHITE);
        }
    }
}

Board::Board(Vector2 size) {
    this->size = size;
}