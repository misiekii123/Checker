#include "raygui.h"
#include "raylib.h"
#include <iostream>
#include "board/board.h"

int main() {
    // Initialize the window
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Checker");

    Board board(Vector2{8, 8});

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        BeginDrawing();
        board.drawBoard();
        ClearBackground(BLACK);
        EndDrawing();
    }

    CloseWindow();
}