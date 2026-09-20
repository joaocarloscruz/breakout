#pragma once
#include <SFML/Graphics.hpp>

class MainMenu {
public:
    explicit MainMenu(const sf::Font& font);
    void start(sf::RenderWindow& window);
    void draw(sf::RenderTarget& target) const;
private:
    const sf::Font& _font;
    sf::Text _title;
    sf::Text _subtitle;
    sf::Text _start;
    sf::Text _exit;
    sf::Text _help;
};
