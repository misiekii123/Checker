#include "ui.h"

void Ui::drawMenu() {
    DrawText("CHECKER", 20, 20, 20, WHITE);

    DrawText("Press ENTER to start", 20, 60, 20, WHITE);
    // Rectangle playButton = {200.0f, 200.0f, 200.0f, 80.0f};

    // if (GuiButton(playButton, "PLAY")) {
    //     // Start the game when the button is clicked
    // }
}

void Ui::drawPauseMenu() {
    DrawText("[PAUSE MENU]", 350, 200, 20, WHITE);
}