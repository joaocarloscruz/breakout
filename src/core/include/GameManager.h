#ifndef CORE_GAME_MANAGER
#define CORE_GAME_MANAGER

#include "core/include/Paddle.h"
#include "core/include/Ball.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include "main.h"

class GameManager {
public:
    GameManager(sf::RenderWindow& window);
    void start();

private:
    sf::RenderWindow& _window;
    Paddle _paddle{SCREEN_WIDTH / 2.f, SCREEN_HEIGHT * 0.9f};
    Ball _ball{SCREEN_WIDTH / 2.f, (SCREEN_HEIGHT / 2.f )+ 150, BALL_RADIUS};

};

#endif // CORE_GAME_MANAGER