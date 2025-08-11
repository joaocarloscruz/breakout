#include "include/Ball.h"

Ball::Ball(float x, float y, float radius) {
    _shape.setRadius(radius);
    _shape.setPosition(sf::Vector2f(x, y));
    _velocity = sf::Vector2f(0.f, 0.f);
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(_shape);
}
