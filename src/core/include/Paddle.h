#ifndef CORE_PADDLE
#define CORE_PADDLE

#include <SFML/Graphics.hpp>

class Paddle {
public:
    Paddle(float x, float y, float width = 100.f, float height = 20.f); //to-do : review sizes
    void move(float dx);
    //void update(float x, float y);
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape _shape;
};

#endif // CORE_PADDLE