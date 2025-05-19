#include "raygui.h"
#include "raylib.h"
#include <iostream>
#include "game/game.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 800;
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "Checker");

    Game game;
    
    SetTargetFPS(60);
    SetExitKey(0);

    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(BLACK);
        game.mainLoop();
        EndDrawing();
    }

    CloseWindow();
}