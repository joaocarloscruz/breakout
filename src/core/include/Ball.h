#ifndef CORE_BALL
#define CORE_BALL

#include <SFML/Graphics.hpp>

const float BALL_RADIUS = 10.f;

class Ball {
public:
    Ball(float x, float y, float radius = BALL_RADIUS);
    void draw(sf::RenderWindow& window);
    /*void update(float deltaTime, float leftLimit, float rightLimit, float topLimit, float bottomLimit);
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

    void setVelocity(float vx, float vy);
    void reverseX();
    void reverseY();*/

private:
    sf::CircleShape _shape;
    sf::Vector2f _velocity;
};

#endif // CORE_BALL