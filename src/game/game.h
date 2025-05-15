#include <raylib.h>

enum class GameState{
    InGame,
    InMenu
};

class Game{
    public:
        void changeGameState();

};