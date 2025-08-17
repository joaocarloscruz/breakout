#include "include/GameManager.h"
#include "main.h"
#include <cmath>

const float wallThickness = 15.f;
const float topLimit = 15.f;
const float leftLimit = 0.20f * SCREEN_WIDTH + 15.f; 
const float rightLimit = 0.80f * SCREEN_WIDTH - 15.f; 

const float containerWidth = rightLimit - leftLimit; 

GameManager::GameManager(sf::RenderWindow& window, sf::Font& font) : 
    _window(window), 
    _brickManager(leftLimit, topLimit + 50.f, NUMBER_ROWS, NUMBER_COLUMNS, 8.f), 
    _status(font),
    _pauseButton(font, "PAUSE", 24) 
{
    _pauseButton.setFillColor(sf::Color::White);
    sf::FloatRect buttonBounds = _pauseButton.getLocalBounds();
    _pauseButton.setPosition(sf::Vector2f(SCREEN_WIDTH - buttonBounds.size.x - 30.f, 10.f));
}

void GameManager::start() {
    auto windowSize = _window.getSize();
    sf::Clock clock; // For delta time calculation

    // Create game boundaries
    sf::RectangleShape topBar(sf::Vector2f(static_cast<float>(windowSize.x * 0.60f), 15.f));
    topBar.setFillColor(sf::Color::White);
    topBar.setPosition(sf::Vector2f((0.20f * SCREEN_WIDTH), 0.f));

    sf::RectangleShape leftBar(sf::Vector2f(15.f, static_cast<float>(windowSize.y)));
    leftBar.setFillColor(sf::Color::White);
    leftBar.setPosition(sf::Vector2f(0.20f * SCREEN_WIDTH, 0.f));

    sf::RectangleShape rightBar(sf::Vector2f(15.f, static_cast<float>(windowSize.y)));
    rightBar.setFillColor(sf::Color::White);
    rightBar.setPosition(sf::Vector2f((static_cast<float>(windowSize.x) - 15.f) - (0.20f * SCREEN_WIDTH), 0.f));

    // Game loop
    while (_window.isOpen()) {
        float deltaTime = clock.restart().asSeconds(); // time between two frames (current - previous)
        // handle events
        while (std::optional<sf::Event> event = _window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                _window.close();
            }
            else if (event->is<sf::Event::KeyPressed>()) {
                if (auto keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyEvent->code == sf::Keyboard::Key::Escape) {
                        _window.close();
                    }
                    if (keyEvent->code == sf::Keyboard::Key::P) {
                        _isPaused = !_isPaused;
                    }
                }
            }
            else if (event->is<sf::Event::MouseButtonPressed>()) {
                if (auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        if (_pauseButton.getGlobalBounds().contains(sf::Vector2f(mouseEvent->position.x, mouseEvent->position.y))) {
                            _isPaused = !_isPaused;
                        }
                    }
                }
            }
        }

        if (!_isPaused) {
            // continuous input
            float paddleDirection = 0.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
                paddleDirection = -1.f;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
                paddleDirection = 1.f;
            }

            _paddle.setMovement(paddleDirection);
            _paddle.update(deltaTime, leftLimit, rightLimit);

            _ball.update(deltaTime);

            sf::Vector2f ballPosition = _ball.getPosition();
            sf::FloatRect ballBounds = _ball.getBounds();

            // Collision with left or right wall
            if (ballPosition.x <= leftLimit || ballBounds.position.x + ballBounds.size.x >= rightLimit) {
                _ball.reverseX();
            }

            // Collision with top wall
            if (ballPosition.y <= topLimit ) {
                _ball.reverseY();
            }

            if (ballPosition.y >= _window.getSize().y) {
                _status.loseLife();
                // Reset ball and paddle positions
                _ball.reset();
                _paddle.reset(PADDLE_POSITION_X, PADDLE_POSITION_Y);
            }

            // collision with paddle
            if (ballBounds.findIntersection(_paddle.getBounds())) {
                _ball.reverseY();
                
                float halfPaddleWidth = PADDLE_WIDTH / 2.0f;
                float paddleCenter = _paddle.getPosition().x + halfPaddleWidth;
                float ballCenter = ballPosition.x + ballBounds.size.x / 2;
                float offset = ballCenter - paddleCenter;
                
                // Normalize offset into a -1, 1 range
                float normalizedOffset = offset / halfPaddleWidth;
                
                sf::Vector2f currentVelocity = _ball.getVelocity();
                float speed = std::sqrt(currentVelocity.x * currentVelocity.x + currentVelocity.y * currentVelocity.y); 
                
                float bounceAngle = 0.0f;
                if (normalizedOffset <= -0.75f) {        // Far-left -60 degrees
                    bounceAngle = -60.0f * M_PI / 180.0f;
                }
                else if (normalizedOffset <= -0.5f) {    // Left -45 degrees  
                    bounceAngle = -45.0f * M_PI / 180.0f;
                }
                else if (normalizedOffset <= -0.25f) {   // Center-left -30 degrees
                    bounceAngle = -30.0f * M_PI / 180.0f;
                }
                else if (normalizedOffset <= 0.25f) {    // Center straight up
                    bounceAngle = 0.0f;
                }
                else if (normalizedOffset <= 0.5f) {     // Center-right 30 degrees
                    bounceAngle = 30.0f * M_PI / 180.0f;
                }
                else if (normalizedOffset <= 0.75f) {    // Right 45 degrees
                    bounceAngle = 45.0f * M_PI / 180.0f;
                }
                else {                                   // Far-right 60 degrees
                    bounceAngle = 60.0f * M_PI / 180.0f;
                }
                
                // Calculate new velocity components
                float newVx = speed * std::sin(bounceAngle);
                float newVy = -speed * std::cos(bounceAngle); 
                _ball.setVelocity(newVx, newVy);
                
                // Move ball slightly above paddle to prevent multiple collisions
                sf::FloatRect paddleBounds = _paddle.getBounds();
                float ballRadius = BALL_RADIUS;
                _ball.setPosition(ballCenter - ballRadius, paddleBounds.position.y - 2 * ballRadius);
            }

            // TO-DO: improve collision system. Fix bug where many bricks are broken at the same time
            std::vector<Brick>& bricks = _brickManager.getBricks();
            for (auto brick = bricks.begin(); brick != bricks.end();) {
                sf::FloatRect brickBounds = brick->getBounds();

                if (ballBounds.findIntersection(brickBounds)) {
                    // Distances from edges
                    float fromLeft   = std::abs((ballBounds.position.x + ballBounds.size.x) - brickBounds.position.x);
                    float fromRight  = std::abs(ballBounds.position.x - (brickBounds.position.x + brickBounds.size.x));
                    float fromTop    = std::abs((ballBounds.position.y + ballBounds.size.y) - brickBounds.position.y);
                    float fromBottom = std::abs(ballBounds.position.y - (brickBounds.position.y + brickBounds.size.y));

                    float minOverlap = std::min({fromLeft, fromRight, fromTop, fromBottom});

                    if (minOverlap == fromLeft) {
                        _ball.reverseX(); // Hit left side
                    }
                    else if (minOverlap == fromRight) {
                        _ball.reverseX(); // Hit right side
                    }
                    else if (minOverlap == fromTop) {
                        _ball.reverseY(); // Hit top side
                    }
                    else if (minOverlap == fromBottom) {
                        _ball.reverseY(); // Hit bottom side
                    }

                    // Remove brick after collision
                    _status.updateScore(1); 
                    brick = bricks.erase(brick);
                    break;
                } 
                else {
                    brick++;
                }
            }
        }

        // render 
        _window.clear();
        _window.draw(topBar);
        _window.draw(leftBar);
        _window.draw(rightBar);
        _status.draw(_window);
        _window.draw(_pauseButton);
        _paddle.draw(_window);
        _ball.draw(_window);
        _brickManager.draw(_window);

        if (_isPaused) {
            sf::Text pauseText(_status.getFont(), "PAUSED", 50);
            pauseText.setFillColor(sf::Color::White);
            sf::FloatRect textBounds = pauseText.getLocalBounds();
            pauseText.setPosition(sf::Vector2f((_window.getSize().x - textBounds.size.x) / 2.f,(_window.getSize().y - textBounds.size.y) / 2.f));
            _window.draw(pauseText);
        }

        _window.display();
    }
}