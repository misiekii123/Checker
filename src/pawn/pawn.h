#include <raylib.h>

enum class PawnColor {
    White,
    Black
};

class Pawn {
private:
    Vector2 position;
public:
    bool is_alive;
    Vector2 getPosition();
    void changePosition(Vector2 position);
    Pawn();
    bool is_queen;
    bool is_alive;
    PawnColor pawn_color;
};