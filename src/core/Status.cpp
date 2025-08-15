#include "include/Status.h"

Status::Status(const sf::Font& font) : _score(font, "Score: 0", 24), _livesText(font, "Lives: 3", 24) {
    _lives = 3;
    _points = 0;
    
    _score.setFillColor(sf::Color::White);
    _score.setPosition(sf::Vector2f(10.f, 10.f));

    _livesText.setFillColor(sf::Color::White);
    _livesText.setPosition(sf::Vector2f(10.f, 40.f));
}

void Status::update() {
    _livesText.setString("Lives: " + std::to_string(_lives));
}

void Status::updateScore(int points) {
    _points += points;
    _score.setString("Score: " + std::to_string(_points));
}

void Status::draw(sf::RenderWindow& window) {
    window.draw(_score);
    window.draw(_livesText);
}

void Status::loseLife() {
    if (_lives > 0) {
        _lives--;
        _livesText.setString("Lives: " + std::to_string(_lives));
    }
}