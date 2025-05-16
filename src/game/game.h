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
    Game();
};