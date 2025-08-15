#ifndef CORE_PADDLE
#define CORE_PADDLE

#include <SFML/Graphics.hpp>

const float PADDLE_WIDTH = 100.f;
const float PADDLE_HEIGHT = 20.f;
const float PADDLE_SPEED = 500.f;

class Paddle {
public:
    Paddle(float x, float y, float width = PADDLE_WIDTH, float height = PADDLE_HEIGHT);
    void update(float deltaTime, float leftLimit, float rightLimit);
    void setMovement(float direction); // -1 for left, 1 for right, 0 for stop
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void reset();

private:
    void clampToBounds(float leftLimit, float rightLimit);
    
    sf::RectangleShape _shape;
    float _movementDirection = 0.f;
    float _speed = PADDLE_SPEED;
};

#endif // CORE_PADDLE