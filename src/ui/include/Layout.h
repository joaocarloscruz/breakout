#pragma once
#include <SFML/Graphics.hpp>
#include "main.h"

inline void centerText(sf::Text& text, float x, float y) {
    const auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                    bounds.position.y + bounds.size.y / 2.f});
    text.setPosition({x, y});
}

inline void updateView(sf::RenderWindow& window) {
    const auto size = window.getSize();
    if (size.x == 0 || size.y == 0) return;
    sf::View view(sf::FloatRect({0.f, 0.f}, {SCREEN_WIDTH, SCREEN_HEIGHT}));
    const float ratio = static_cast<float>(size.x) / static_cast<float>(size.y);
    const float designRatio = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;
    if (ratio > designRatio) {
        const float width = designRatio / ratio;
        view.setViewport(sf::FloatRect({(1.f - width) / 2.f, 0.f}, {width, 1.f}));
    } else {
        const float height = ratio / designRatio;
        view.setViewport(sf::FloatRect({0.f, (1.f - height) / 2.f}, {1.f, height}));
    }
    window.setView(view);
}
