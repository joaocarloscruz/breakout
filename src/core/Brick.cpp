#include "include/Brick.h"

Brick::Brick(float x, float y, float width, float height, sf::Color color){
    _shape.setPosition(sf::Vector2f(x, y));
    _shape.setSize(sf::Vector2f(width, height));
    _shape.setFillColor(color);
}

sf::FloatRect Brick::getBounds() const {
    return _shape.getGlobalBounds();
}

void Brick::draw(sf::RenderWindow& window) const {
    window.draw(_shape);
}

bool Brick::isDestroyed() {
    return _destroyed;
}

int Brick::getHitsRemaining() {
    return _hitsRemaining;
}
