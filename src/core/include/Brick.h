#ifndef CORE_BRICK
#define CORE_BRICK

#include <SFML/Graphics.hpp>

const float BRICK_WIDTH = 60.f;
const float BRICK_HEIGHT = 20.f;

class Brick {
public:
    Brick(float x, float y, float width, float height, sf::Color color);
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    bool isDestroyed();
    int getHitsRemaining(); // As of the current implementation, each brick will only take one hit.

private:
    sf::RectangleShape _shape;
    int _hitsRemaining;
    bool _destroyed;
};

#endif // CORE_BRICK