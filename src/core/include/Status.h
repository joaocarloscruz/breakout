#ifndef CORE_STATUS
#define CORE_STATUS

#include <SFML/Graphics.hpp>

class Status {
public:
    Status(const sf::Font& font);
    void update();
    void draw(sf::RenderWindow& window);
    void updateScore(int points);
    bool loseLife();
    const sf::Font& getFont() const;

private:
    const sf::Font& _font;
    sf::Text _score;
    sf::Text _livesText;
    int _lives;
    int _points;
};

#endif // CORE_STATUS