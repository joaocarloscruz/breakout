#include "core/include/Paddle.h"

Paddle::Paddle(float x, float y, float width, float height) {
    _shape.setSize(sf::Vector2f(width, height));
    _shape.setPosition(sf::Vector2f(x, y));
}

void Paddle::draw(sf::RenderWindow& window) {
    window.draw(_shape);
}

void Paddle::move(float dx){
    _shape.move(sf::Vector2f(dx, 0));
}