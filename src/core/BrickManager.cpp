#include "include/BrickManager.h"
#include <stdio.h>

BrickManager::BrickManager(float startX, float startY, int rows, int columns, float spacing) {
    _spacing = spacing;
    createLevel(rows, columns, startX, startY);
}

void BrickManager::createLevel(int rows, int columns, float startX, float startY) {
    const float BRICK_WIDTH = 60.0f; 
    const float BRICK_HEIGHT = 20.0f;
    const float PADDING_TOP = 20.0f + startY; // Starting y position
    const float PADDING_LEFT = 30.0f + startX ; // Starting x position
    sf::Color brickColor;
    for (int i = 0; i < rows; ++i) {
        brickColor = chooseColor(i);
        for (int j = 0; j < columns; ++j) {
            float posX = PADDING_LEFT + (j * (BRICK_WIDTH + _spacing));
            float posY = PADDING_TOP + (i * (BRICK_HEIGHT + _spacing));

            _bricks.emplace_back(posX, posY, BRICK_WIDTH, BRICK_HEIGHT, brickColor);
        }
    }
}

sf::Color BrickManager::chooseColor(int i){
    switch (i % 4) {
        case 0: return sf::Color::Cyan;
        case 1: return sf::Color::Red;
        case 2: return sf::Color::Green;
        case 3: return sf::Color::Yellow;
        default: return sf::Color::White; 
    }
}

void BrickManager::draw(sf::RenderWindow& window){
    for (const auto& brick : _bricks) {
        brick.draw(window);
    }
}

std::vector<Brick>& BrickManager::getBricks() {
    return _bricks;
}
