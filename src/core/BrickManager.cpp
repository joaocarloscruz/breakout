#include "include/BrickManager.h"
#include <algorithm>
#include <array>

void BrickManager::loadLevel(const Level& level, float left, float right, float top) {
    _bricks.clear();
    constexpr float spacing = 8.f;
    const std::array colors{sf::Color::Cyan, sf::Color::Red, sf::Color::Green, sf::Color::Yellow};
    std::size_t columns = 0;
    for (const auto& row : level.rows) columns = std::max(columns, row.size());
    if (columns == 0) return;
    const float width = static_cast<float>(columns) * (BRICK_WIDTH + spacing) - spacing;
    const float startX = left + (right - left - width) / 2.f;
    for (std::size_t row = 0; row < level.rows.size(); ++row) {
        for (std::size_t col = 0; col < level.rows[row].size(); ++col) {
            const char cell = level.rows[row][col];
            if (cell == '.') continue;
            _bricks.emplace_back(startX + static_cast<float>(col) * (BRICK_WIDTH + spacing),
                top + static_cast<float>(row) * (BRICK_HEIGHT + spacing),
                BRICK_WIDTH, BRICK_HEIGHT, colors[row % colors.size()], cell == '2' ? 2 : 1);
        }
    }
}
void BrickManager::draw(sf::RenderTarget& target) const {
    for (const auto& brick : _bricks) brick.draw(target);
}
std::vector<Brick>& BrickManager::getBricks() { return _bricks; }
const std::vector<Brick>& BrickManager::getBricks() const { return _bricks; }
int BrickManager::getBricksRemaining() const {
    return static_cast<int>(std::count_if(_bricks.begin(), _bricks.end(),
        [](const Brick& brick) { return !brick.isDestroyed(); }));
}
bool BrickManager::allBricksDestroyed() const { return getBricksRemaining() == 0; }
