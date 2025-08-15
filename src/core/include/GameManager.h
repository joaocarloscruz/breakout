#ifndef CORE_GAME_MANAGER
#define CORE_GAME_MANAGER

#include "core/include/Paddle.h"
#include "core/include/Ball.h"
#include "core/include/BrickManager.h"
#include "core/include/Status.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include "main.h"

const float PADDLE_POSITION_X = (SCREEN_WIDTH / 2.f);
const float PADDLE_POSITION_Y = (SCREEN_HEIGHT * 0.9f);

class GameManager {
public:
    GameManager(sf::RenderWindow& window, sf::Font& font);
    void start();

private:
    sf::RenderWindow& _window;
    Paddle _paddle{PADDLE_POSITION_X, PADDLE_POSITION_Y};
    Ball _ball{SCREEN_WIDTH / 2.f, (SCREEN_HEIGHT / 2.f )+ 150, BALL_RADIUS};
    BrickManager _brickManager;
    Status _status;

};

#endif // CORE_GAME_MANAGER