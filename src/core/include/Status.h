#pragma once
#include "GameSession.h"
#include <SFML/Graphics.hpp>

class Status {
public:
    explicit Status(const sf::Font& font);
    void draw(sf::RenderTarget& target, const GameSession& session);
private:
    sf::Text _score;
    sf::Text _lives;
    sf::Text _level;
    sf::Text _remaining;
};
