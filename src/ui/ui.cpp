#include "ui.h"

void Ui::drawMenu() {
    DrawText("CHECKER", 20, 20, 40, WHITE);

    DrawText("Press 1 to play in singleplayer mode", 20, 80, 20, WHITE);
    DrawText("Press 2 to play in multiplayer mode", 20, 110, 20, WHITE);
    // Rectangle playButton = {200.0f, 200.0f, 200.0f, 80.0f};

    // if (GuiButton(playButton, "PLAY")) {
    //     // Start the game when the button is clicked
    // }
}

void Ui::drawPauseMenu() {
    DrawText("PAUSE", 20, 20, 40, WHITE);
    DrawText("Press enter to play return", 20, 80, 20, WHITE);
    DrawText("Press escape to exit to the main menu", 20, 110, 20, WHITE);
}

void Ui::drawGameOver(Color winningColor) {
    DrawText("GAME OVER", 20, 20, 40, WHITE);
    DrawText("Press escape to exit to the main menu", 20, 80, 20, WHITE);
    if (ColorIsEqual(winningColor, WHITE)) {
        DrawText("WHITE WINS!", 20, 110, 20, WHITE);
    } else if (ColorIsEqual(winningColor, BLACK)) {
        DrawText("BLACK WINS!", 20, 110, 20, WHITE);
    } else {
        DrawText("DRAW!", 20, 110, 20, GRAY);
    }
}