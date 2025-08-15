#ifndef CORE_BALL
#define CORE_BALL

#include <SFML/Graphics.hpp>

const float BALL_RADIUS = 10.f;

class Ball {
public:
    Ball(float x, float y, float radius = BALL_RADIUS);
    void draw(sf::RenderWindow& window);
    void update(float deltaTime);
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

    void setVelocity(float vx, float vy);      
    sf::Vector2f getVelocity() const;          
    void reverseX();
    void reverseY();
    void setPosition(float x, float y);

    void reset();

private:
    sf::CircleShape _shape;
    sf::Vector2f _velocity;
};

#endif // CORE_BALL