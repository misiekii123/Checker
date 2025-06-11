#include "board.h"

void Board::drawBoard() {
    for (int row = 0; row < size.x; ++row) {
        for (int col = 0; col < size.y; ++col) {
            if ((row + col) % 2 == 0) {
                DrawRectangle(row * 100, col * 100, 100, 100, GRAY);
            } else {
                DrawRectangle(row * 100, col * 100, 100, 100, DARKGRAY);
            }
        }
    }
}

void Board::fillPawns() {
    for (int row = 0; row < size.x; ++row) {
        for (int col = 0; col < size.y; ++col) {
            if (board[row][col] != nullptr) {
                delete board[row][col]; 
                board[row][col] = nullptr;
            }
        }
    }

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < size.y; ++col) {
            if ((row + col) % 2 == 1) {
                Vector2 pos = { float(col * 100 + 50), float(row * 100 + 50) };
                board[row][col] = new Pawn(BLACK, pos);
            }
        }
    }

    for (int row = 5; row < size.x; ++row) {
        for (int col = 0; col < size.y; ++col) {
            if ((row + col) % 2 == 1) {
                Vector2 pos = { float(col * 100 + 50), float(row * 100 + 50) };
                board[row][col] = new Pawn(WHITE, pos);
            }
        }
    }
}

void Board::drawPawns() {
    for (int row = 0; row < size.x; ++row) {
        for (int col = 0; col < size.y; ++col) {
            Pawn* pawn = board[row][col];
            if (pawn != nullptr && pawn->is_alive) {
                Color color = ColorIsEqual(pawn->pawn_color, BLACK) ? BLACK : WHITE;
                DrawCircleV(pawn->getPosition(), 40, color);
                if(pawn->is_queen) {
                    DrawRing(pawn->getPosition(), 40, 45, 0, 360, 1, PINK);
                }
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