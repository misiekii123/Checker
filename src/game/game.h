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
    bool pawn_selected = false;
    Pawn* selectedPawn = nullptr;
    void mouseControl();
    void drawPawnSelection(Pawn* pawn);
    void drawAwailableBeating(std::vector<Vector2> availableBeating);
    void drawAwailableMoves(std::vector<std::vector<Vector2>> availableMoves);    
    void simulateMultiBeating(Pawn* pawn, std::vector<Vector2> current_path, std::vector<std::vector<Vector2>>& all_paths);
    void simulateMultiBeatingInternal(Pawn* tempBoard[8][8], int x, int y, std::vector<Vector2> current_path, std::vector<std::vector<Vector2>>& all_paths, Color color);
    bool isInsideBoard(int x, int y);
public:
    void changeGameState(GameState newState);
    void startGame();
    void mainLoop();
    Game();
    std::vector<std::vector<Vector2>> legalMoves(Pawn* pawn);
    std::vector<Vector2> whereIsBeatingAvailable(Pawn* pawn);
    std::vector<std::vector<Vector2>> multipleBeatings(Pawn* pawn);
};