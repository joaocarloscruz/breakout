#include "include/Brick.h"
#include <algorithm>

Brick::Brick(float x, float y, float width, float height, sf::Color color, int hits)
    : _color(color), _hitsRemaining(std::max(1, hits)) {
    _shape.setPosition({x, y});
    _shape.setSize({width, height});
    _shape.setFillColor(color);
    _shape.setOutlineColor(sf::Color::White);
    _shape.setOutlineThickness(_hitsRemaining > 1 ? -2.f : 0.f);
}

sf::FloatRect Brick::getBounds() const { return _shape.getGlobalBounds(); }
bool Brick::isDestroyed() const { return _hitsRemaining == 0; }
int Brick::getHitsRemaining() const { return _hitsRemaining; }

bool Brick::hit() {
    if (isDestroyed()) return false;
    --_hitsRemaining;
    _shape.setOutlineThickness(_hitsRemaining > 1 ? -2.f : 0.f);
    // The border disappears and the color softens when armor breaks.
    _shape.setFillColor(sf::Color(_color.r, _color.g, _color.b, 210));
    return isDestroyed();
}

void Brick::draw(sf::RenderTarget& target) const {
    if (!isDestroyed()) target.draw(_shape);
}
