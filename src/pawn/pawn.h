#include <raylib.h>

enum class PawnColor {
    White,
    Black
};

class Pawn {
private:
    Vector2 position;
    PawnColor pawnColor;
    bool IsAlive;
public:
    Vector2 getPosition();
    void changePosition(Vector2 position);
};