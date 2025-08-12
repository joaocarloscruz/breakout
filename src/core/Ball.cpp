#include "include/Ball.h"

Ball::Ball(float x, float y, float radius) {
    _shape.setRadius(radius);
    _shape.setPosition(sf::Vector2f(x, y));
    _shape.setFillColor(sf::Color::White);  
    _velocity = sf::Vector2f(300.f, -300.f); // ball starts going upwards
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(_shape);
}

void Ball::update(float deltaTime){
    _shape.move(_velocity * deltaTime);
}

sf::FloatRect Ball::getBounds() const {
    return _shape.getGlobalBounds();
}

sf::Vector2f Ball::getPosition() const {
    return _shape.getPosition();
}

sf::Vector2f Ball::getVelocity() const {    
    return _velocity;
}

void Ball::setVelocity(float vx, float vy) {
    _velocity = sf::Vector2f(vx, vy);
}

void Ball::reverseX() {
    _velocity.x = -_velocity.x;
}

void Ball::reverseY() {
    _velocity.y = -_velocity.y;
}

void Ball::setPosition(float x, float y) {    
    _shape.setPosition(sf::Vector2f(x, y));
}