#pragma once
#include "GameSession.h"
#include "Status.h"
#include <SFML/Graphics.hpp>

class GameManager {
public:
    explicit GameManager(const sf::Font& font);
    void start(sf::RenderWindow& window);
    void draw(sf::RenderTarget& target);

private:
    friend struct GameManagerTestAccess;
    const sf::Font& _font;
    GameSession _session;
    Status _status;
    sf::Text _pauseButton;
    sf::Text _help;
    void drawOverlay(sf::RenderTarget& target, const std::string& title,
                     const std::string& detail, sf::Color color = sf::Color::White);
};
