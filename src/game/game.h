#include <raylib.h>

enum class GameState{
    InGame,
    InMenu,
    InPause
};

class Game{
    public:
        void changeGameState();

};