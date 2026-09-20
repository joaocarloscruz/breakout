#pragma once
#include "Ball.h"
#include "Paddle.h"
#include "BrickManager.h"
#include "main.h"
#include <cstddef>

namespace arena {
inline constexpr float left = 0.20f * SCREEN_WIDTH + 15.f;
inline constexpr float right = 0.80f * SCREEN_WIDTH - 15.f;
inline constexpr float top = 15.f;
inline constexpr float paddleY = 0.9f * SCREEN_HEIGHT;
inline constexpr float step = 1.f / 240.f;
}

enum class PlayState { Ready, Playing, Paused, LevelComplete, Won, GameOver };

// Gameplay has no window, event polling, fonts, or wall-clock dependencies.
class GameSession {
public:
    GameSession();
    void restart();
    void advance(); // Space/Enter: launch, next level, or new campaign.
    void togglePause();
    void pause();
    void update(float elapsedSeconds, float direction);

    PlayState state() const { return _state; }
    int score() const { return _score; }
    int lives() const { return _lives; }
    std::size_t levelIndex() const { return _levelIndex; }
    const Level& level() const { return campaignLevels()[_levelIndex]; }
    const Ball& ball() const { return _ball; }
    const Paddle& paddle() const { return _paddle; }
    const BrickManager& bricks() const { return _bricks; }

private:
    friend struct GameSessionTestAccess;
    Ball _ball{0.f, 0.f};
    Paddle _paddle{SCREEN_WIDTH / 2.f, arena::paddleY};
    BrickManager _bricks;
    PlayState _state = PlayState::Ready;
    PlayState _beforePause = PlayState::Ready;
    std::size_t _levelIndex = 0;
    int _lives = 3;
    int _score = 0;
    float _accumulator = 0.f;

    void loadLevel();
    void prepareServe();
    void attachBall();
    void step(float direction);
    void collideBrick(Brick& brick, const sf::FloatRect& previous);
};
