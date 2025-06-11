#pragma once

#include <raylib.h>
#include "raygui.h"

class Ui {
public:
    void drawMenu();
    void drawPauseMenu();
    void drawGameOver(Color winningColor);
};