#include "core/include/Paddle.h"

Paddle::Paddle(float x, float y, float width, float height) {
    _shape.setSize(sf::Vector2f(width, height));
    _shape.setPosition(sf::Vector2f(x - width/2.f, y)); 
    _shape.setFillColor(sf::Color::White);
}

void Paddle::update(float deltaTime, float leftLimit, float rightLimit) {
    // Only move if there's a movement direction set
    if (_movementDirection != 0.f) {
        float displacement = _movementDirection * _speed * deltaTime;
        _shape.move(sf::Vector2f(displacement, 0.f));
        
        clampToBounds(leftLimit, rightLimit);
    }
}

void Paddle::setMovement(float direction) {
    _movementDirection = direction; 
}

void Paddle::draw(sf::RenderWindow& window) {
    window.draw(_shape);
}

sf::FloatRect Paddle::getBounds() const {
    return _shape.getGlobalBounds();
}

sf::Vector2f Paddle::getPosition() const {
    return _shape.getPosition();
}

void Paddle::clampToBounds(float leftLimit, float rightLimit) {
    sf::Vector2f position = _shape.getPosition();
    sf::Vector2f size = _shape.getSize();
    
    // Ensure paddle doesn't go past left limit
    if (position.x < leftLimit) {
        position.x = leftLimit;
    }
    // Ensure paddle doesn't go past right limit
    else if (position.x + size.x > rightLimit) {
        position.x = rightLimit - size.x;
    }
    
    _shape.setPosition(position);
}

void Paddle::reset(){
    _shape.setPosition(sf::Vector2f(400.f, 550.f));
}