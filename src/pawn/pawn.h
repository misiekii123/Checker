#include <raylib.h>

class Pawn {
private:
    Vector2 position;
public:
    Vector2 getPosition();
    void changePosition(Vector2 position);
};