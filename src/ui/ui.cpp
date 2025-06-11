#include "ui.h"

void Ui::drawMenu() {
    DrawText("CHECKER", 20, 20, 20, WHITE);

    DrawText("Press 1 to play in singleplayer mode", 20, 60, 20, WHITE);
    DrawText("Press 2 to play in multiplayer mode", 20, 80, 20, WHITE);
    // Rectangle playButton = {200.0f, 200.0f, 200.0f, 80.0f};

    // if (GuiButton(playButton, "PLAY")) {
    //     // Start the game when the button is clicked
    // }
}

void Ui::drawPauseMenu() {
    DrawText("[PAUSE MENU]", 350, 200, 20, WHITE);
}