#include "raygui.h"
#include "raylib.h"
#include <iostream>
#include "game/game.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Checker");

    Game game;
    
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        BeginDrawing();
        board.drawBoard();
        ClearBackground(BLACK);
        game.mainLoop();
        EndDrawing();
    }

    CloseWindow();
}