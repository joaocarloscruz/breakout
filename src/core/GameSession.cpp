#include "include/GameSession.h"
#include <algorithm>
#include <cmath>
#include <numbers>

GameSession::GameSession() { restart(); }
void GameSession::restart() {
    _score = 0;
    _lives = 3;
    _levelIndex = 0;
    loadLevel();
}
void GameSession::loadLevel() {
    _bricks.loadLevel(level(), arena::left, arena::right, 85.f);
    prepareServe();
}
void GameSession::attachBall() {
    const auto bounds = _paddle.getBounds();
    _ball.setPosition(bounds.position.x + bounds.size.x / 2.f - BALL_RADIUS,
                      bounds.position.y - 2.f * BALL_RADIUS - 1.f);
}
void GameSession::prepareServe() {
    _paddle.reset(SCREEN_WIDTH / 2.f, arena::paddleY);
    _paddle.setMovement(0.f);
    _ball.setVelocity(0.f, 0.f);
    attachBall();
    _accumulator = 0.f;
    _state = PlayState::Ready;
}
void GameSession::advance() {
    if (_state == PlayState::Ready) {
        const float speed = level().ballSpeed;
        _ball.setVelocity(speed * 0.4f, -speed * std::sqrt(0.84f));
        _state = PlayState::Playing;
    } else if (_state == PlayState::LevelComplete) {
        ++_levelIndex;
        _lives = std::min(_lives + 1, 5);
        loadLevel();
    } else if (_state == PlayState::Won || _state == PlayState::GameOver) {
        restart();
    }
}
void GameSession::pause() {
    if (_state == PlayState::Playing || _state == PlayState::Ready) {
        _beforePause = _state;
        _state = PlayState::Paused;
        _accumulator = 0.f;
    }
}
void GameSession::togglePause() {
    if (_state == PlayState::Paused) {
        _state = _beforePause;
        _accumulator = 0.f;
    } else {
        pause();
    }
}
void GameSession::update(float elapsedSeconds, float direction) {
    if (_state != PlayState::Playing && _state != PlayState::Ready) return;
    if (!std::isfinite(elapsedSeconds) || elapsedSeconds <= 0.f) return;
    // At most 100 ms of catch-up after a stall. Physics always uses small steps.
    _accumulator += std::min(elapsedSeconds, 0.1f);
    while (_accumulator >= arena::step) {
        _accumulator -= arena::step;
        step(std::clamp(direction, -1.f, 1.f));
        if (_state != PlayState::Playing && _state != PlayState::Ready) {
            _accumulator = 0.f;
            break;
        }
    }
}
void GameSession::step(float direction) {
    _paddle.setMovement(direction);
    _paddle.update(arena::step, arena::left, arena::right);
    if (_state == PlayState::Ready) {
        attachBall();
        return;
    }
    const auto previous = _ball.getBounds();
    _ball.update(arena::step);
    auto position = _ball.getPosition();
    auto velocity = _ball.getVelocity();
    const float diameter = 2.f * BALL_RADIUS;
    if (position.x < arena::left) {
        position.x = arena::left;
        velocity.x = std::abs(velocity.x);
    } else if (position.x + diameter > arena::right) {
        position.x = arena::right - diameter;
        velocity.x = -std::abs(velocity.x);
    }
    if (position.y < arena::top) {
        position.y = arena::top;
        velocity.y = std::abs(velocity.y);
    }
    _ball.setPosition(position.x, position.y);
    _ball.setVelocity(velocity.x, velocity.y);

    if (position.y >= SCREEN_HEIGHT) {
        --_lives;
        if (_lives == 0) {
            _state = PlayState::GameOver;
        } else {
            prepareServe();
        }
        return; // Never collide using the pre-reset ball bounds.
    }

    const auto paddleBounds = _paddle.getBounds();
    if (velocity.y > 0.f && previous.position.y + diameter <= paddleBounds.position.y + 1.f &&
        _ball.getBounds().findIntersection(paddleBounds)) {
        const float center = position.x + BALL_RADIUS;
        const float paddleCenter = paddleBounds.position.x + paddleBounds.size.x / 2.f;
        float offset = std::clamp((center - paddleCenter) / (paddleBounds.size.x / 2.f), -1.f, 1.f);
        // Avoid an endless vertical lane after a perfectly centered bounce.
        if (std::abs(offset) < 0.08f) offset = velocity.x < 0.f ? -0.08f : 0.08f;
        const float angle = offset * std::numbers::pi_v<float> / 3.f;
        const float speed = level().ballSpeed;
        _ball.setVelocity(speed * std::sin(angle), -speed * std::cos(angle));
        _ball.setPosition(position.x, paddleBounds.position.y - diameter - 0.01f);
    }

    for (auto& brick : _bricks.getBricks()) {
        if (brick.isDestroyed() || !_ball.getBounds().findIntersection(brick.getBounds())) continue;
        collideBrick(brick, previous);
        _score += brick.hit() ? 100 : 25;
        break; // Resolve a single contact, then recompute bounds on the next step.
    }
    if (_bricks.allBricksDestroyed()) {
        _score += 500;
        _state = _levelIndex + 1 == campaignLevels().size() ? PlayState::Won : PlayState::LevelComplete;
    }
}
void GameSession::collideBrick(Brick& brick, const sf::FloatRect& previous) {
    const auto bounds = brick.getBounds();
    const auto ballBounds = _ball.getBounds();
    auto position = _ball.getPosition();
    auto velocity = _ball.getVelocity();
    const float size = 2.f * BALL_RADIUS;
    // Prefer the face actually crossed over the last physics step.
    if (previous.position.y + size <= bounds.position.y) {
        position.y = bounds.position.y - size - 0.01f;
        velocity.y = -std::abs(velocity.y);
    } else if (previous.position.y >= bounds.position.y + bounds.size.y) {
        position.y = bounds.position.y + bounds.size.y + 0.01f;
        velocity.y = std::abs(velocity.y);
    } else if (previous.position.x + size <= bounds.position.x) {
        position.x = bounds.position.x - size - 0.01f;
        velocity.x = -std::abs(velocity.x);
    } else if (previous.position.x >= bounds.position.x + bounds.size.x) {
        position.x = bounds.position.x + bounds.size.x + 0.01f;
        velocity.x = std::abs(velocity.x);
    } else {
        // Recover from an existing overlap by pushing out of the shallowest face.
        const float left = ballBounds.position.x + size - bounds.position.x;
        const float right = bounds.position.x + bounds.size.x - ballBounds.position.x;
        const float top = ballBounds.position.y + size - bounds.position.y;
        const float bottom = bounds.position.y + bounds.size.y - ballBounds.position.y;
        const float overlap = std::min({left, right, top, bottom});
        if (overlap == left) {
            position.x = bounds.position.x - size - 0.01f;
            velocity.x = -std::abs(velocity.x);
        } else if (overlap == right) {
            position.x = bounds.position.x + bounds.size.x + 0.01f;
            velocity.x = std::abs(velocity.x);
        } else if (overlap == top) {
            position.y = bounds.position.y - size - 0.01f;
            velocity.y = -std::abs(velocity.y);
        } else {
            position.y = bounds.position.y + bounds.size.y + 0.01f;
            velocity.y = std::abs(velocity.y);
        }
    }
    _ball.setPosition(position.x, position.y);
    _ball.setVelocity(velocity.x, velocity.y);
}
