#pragma once

#include <raylib.h>
#include "board/board.h"
#include "ui/ui.h"
#include <pawn/pawn.h>
#include <board/board.h>
#include <vector>

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
    bool colorsEqual(Color c1, Color c2);
public:
    void changeGameState(GameState newState);
    void startGame();
    void mainLoop();
    Game();
    bool isMoveLegal(Pawn pawn, Vector2 new_position, std::vector <std::vector<bool>> fields());
    bool isBeatingAvailable(Pawn* pawn);
};