#pragma once

#include <raylib.h>
#include "board/board.h"
#include "ui/ui.h"

enum class GameState {
    InGame,
    InMenu,
    InPause
};

class Game {
private:
    GameState currentState;
    Ui ui;
    Board board;
public:
    void changeGameState(GameState newState);
    void startGame();
    void mainLoop();
    Game();
};