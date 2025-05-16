#include <raylib.h>

enum class Color {
    White,
    Black
};

class Pawn {
private:
    Vector2 position;
    Color pawnColor;
    bool IsAlive;
public:
    Vector2 getPosition();
    void changePosition(Vector2 position);
};