#ifndef CORE_BRICK_MANAGER
#define CORE_BRICK_MANAGER

#include "Brick.h"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

const float NUMBER_ROWS = 5;
const float NUMBER_COLUMNS = 10;

class BrickManager {
public: 
    BrickManager(float startX, float startY,  int rows, int columns, float spacing);
    void createLevel(int rows, int columns, float startX, float startY);
    void draw(sf::RenderWindow& window);
    void clear(); 

    std::vector<Brick>& getBricks();

    bool allBricksDestroyed();
    int getBricksRemaining();
    int getTotalBricks();

private:
    std::vector<Brick> _bricks;
    float _startX, _startY, _spacing;
    int _rows, _columns;
    sf::Color chooseColor(int i);
};

#endif // CORE_BRICK_MANAGER