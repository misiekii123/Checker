#pragma once

#include <raylib.h>
#include "board/board.h"
#include "ui/ui.h"
#include "beatings/beatings.h"
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
    Beatings beatings;
    Ui ui;
    Board board;
    bool pawn_selected = false;
    Pawn* selectedPawn = nullptr;
    Color player_color = WHITE; 
    Color enemy_color = BLACK;
    void mouseControl(Beatings* beatings);
    void drawPawnSelection(Pawn* pawn);
    void drawAwailableBeating(std::vector<Vector2> availableBeating);
    void drawAwailableMoves(std::vector<std::vector<Vector2>> availableMoves);    
    bool isInsideBoard(int x, int y);
public:
    void changeGameState(GameState newState);
    void startGame();
    void mainLoop(Beatings* beatings);
    Game();
    
};