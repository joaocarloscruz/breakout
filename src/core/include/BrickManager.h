#pragma once
#include "Brick.h"
#include "Level.h"
#include <vector>

class BrickManager {
public:
    void loadLevel(const Level& level, float left, float right, float top);
    void draw(sf::RenderTarget& target) const;
    std::vector<Brick>& getBricks();
    const std::vector<Brick>& getBricks() const;
    bool allBricksDestroyed() const;
    int getBricksRemaining() const;

private:
    std::vector<Brick> _bricks;
};
