#pragma once

#include <raylib.h>
#include "board/board.h"
#include "ui/ui.h"
#include "beatings/beatings.h"
#include <pawn/pawn.h>
#include <board/board.h>
#include <vector>
#include <utils/utils.h>
#include <ai/ai.h>

enum class Player {
    Player,
    Enemy
};

class Game {
private:
    GameMode gameMode;
    GameState currentState;
    Beatings beatings;
    Ui ui;
    Ai ai;
    Board board;
    Player playerTurn;
    bool pawn_selected = false;
    Pawn* selectedPawn = nullptr;
    Color player_color = WHITE; 
    Color enemy_color = BLACK;
    Color winning_color;
    bool is_finished;
    void mouseControl();
    void drawPawnSelection(Pawn* pawn);
    void drawAwailableBeating(std::vector<Vector2> availableBeating);
    void drawAwailableMoves(std::vector<std::vector<Vector2>> availableMoves);    
    bool isInsideBoard(int x, int y);
    bool isFinished(Color color);
    Pawn* pawnHasBeatingAvailable(Color color);
    bool anyPawnHasBeating(Color color);
public:
    void changeGameState(GameState newState);
    void startGame();
    void mainLoop();
    Game();
};