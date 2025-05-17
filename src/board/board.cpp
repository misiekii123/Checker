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
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board[row][col] != nullptr) {
                delete board[row][col]; 
                board[row][col] = nullptr;
            }
        }
    }

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 8; ++col) {
            if ((row + col) % 2 == 1) {
                Vector2 pos = { col * 100 + 50, row * 100 + 50 };
                board[row][col] = new Pawn(BLACK, pos);
            }
        }
    }

    for (int row = 5; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if ((row + col) % 2 == 1) {
                Vector2 pos = { col * 100 + 50, row * 100 + 50 };
                board[row][col] = new Pawn(WHITE, pos);
            }
        }
    }
}

void Board::drawPawns() {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Pawn* pawn = board[row][col];
            if (pawn != nullptr && pawn->is_alive) {
                Color color = (pawn->pawn_color.r == BLACK.r && pawn->pawn_color.g == BLACK.g && pawn->pawn_color.b == BLACK.b && pawn->pawn_color.a == BLACK.a) ? BLACK : WHITE;
                DrawCircleV(pawn->getPosition(), 40, color);
            }
        }
    }
}

Board::Board(Vector2 size) {
    this->size = size;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            board[row][col] = nullptr;
        }
    }
}

Board::~Board() {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            delete board[row][col];
            board[row][col] = nullptr;
        }
    }
}