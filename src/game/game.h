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
    bool isPawnSelected(Pawn* pawn);
    void mouseControl();
    bool colorsEqual(Color c1, Color c2);
public:
    void changeGameState(GameState newState);
    void startGame();
    void mainLoop();
    Game();
    std::vector<Vector2> legalMoves(Pawn* pawn);
    std::vector<Vector2> whereIsBeatingAvailable(Pawn* pawn);
    void Game::simulateMultiBeating(Pawn* pawn, std::vector<Vector2> current_path, std::vector<Vector2>& best_path);
    std::vector<Vector2> multipleBeatings(Pawn* pawn);
};