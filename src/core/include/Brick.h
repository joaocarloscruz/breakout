#pragma once
#include <SFML/Graphics.hpp>

inline constexpr float BRICK_WIDTH = 60.f;
inline constexpr float BRICK_HEIGHT = 20.f;

class Brick {
public:
    Brick(float x, float y, float width, float height, sf::Color color, int hits = 1);
    void draw(sf::RenderTarget& target) const;
    sf::FloatRect getBounds() const;
    bool isDestroyed() const;
    int getHitsRemaining() const;
    bool hit(); // Returns true only for the hit that destroys this brick.

private:
    sf::RectangleShape _shape;
    sf::Color _color;
    int _hitsRemaining;
};
