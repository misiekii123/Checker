#include <raylib.h>

enum class GameState {
    InGame,
    InMenu,
    InPause
};

class Game {
private:
    GameState currentState;
public:
    void changeGameState(GameState newState);
    void startGame();
    void mainLoop();
    Game();
};