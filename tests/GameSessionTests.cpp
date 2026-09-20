#include "TestAccess.h"
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

#define CHECK(condition) do { if (!(condition)) throw std::runtime_error(#condition); } while (false)
bool near(float a, float b, float tolerance = 0.01f) { return std::abs(a - b) < tolerance; }
using Access = GameSessionTestAccess;

void arrangeBrickHit(GameSession& game, int hits = 1) {
    Access::bricks(game).clear();
    Access::bricks(game).emplace_back(600.f, 200.f, 60.f, 20.f, sf::Color::Cyan, hits);
    Access::ball(game).setPosition(620.f, 222.f);
    Access::ball(game).setVelocity(0.f, -game.level().ballSpeed);
    Access::state(game, PlayState::Playing);
}
void tick(GameSession& game, int count = 1) {
    for (int i = 0; i < count; ++i) game.update(arena::step, 0.f);
}
void loseBall(GameSession& game) {
    game.advance();
    Access::ball(game).setPosition(500.f, SCREEN_HEIGHT + 1.f);
    Access::ball(game).setVelocity(0.f, game.level().ballSpeed);
    tick(game);
}

int main() {
    const std::pair<const char*, std::function<void()>> tests[] = {
        {"six distinct, valid, centered levels with increasing speed", [] {
            CHECK(campaignLevels().size() == 6);
            float speed = 0.f;
            std::vector<std::vector<std::string>> layouts;
            for (const auto& level : campaignLevels()) {
                CHECK(!level.name.empty());
                CHECK(level.ballSpeed > speed && level.ballSpeed <= 600.f);
                speed = level.ballSpeed;
                for (const auto& layout : layouts) CHECK(layout != level.rows);
                layouts.push_back(level.rows);
                int expected = 0;
                for (const auto& row : level.rows) {
                    CHECK(row.size() == 10);
                    for (char cell : row) {
                        CHECK(cell == '.' || cell == '1' || cell == '2');
                        if (cell != '.') ++expected;
                    }
                }
                BrickManager manager;
                manager.loadLevel(level, arena::left, arena::right, 85.f);
                CHECK(expected > 0);
                CHECK(manager.getBricksRemaining() == expected);
                for (const auto& brick : manager.getBricks()) {
                    const auto b = brick.getBounds();
                    CHECK(b.position.x >= arena::left);
                    CHECK(b.position.x + b.size.x <= arena::right);
                    CHECK(b.position.y + b.size.y < 400.f);
                }
                manager.loadLevel(level, arena::left, arena::right, 85.f);
                CHECK(manager.getBricksRemaining() == expected); // no accumulation on reload
            }
        }},
        {"armor needs two hits and cannot score destruction twice", [] {
            Brick brick(0, 0, 60, 20, sf::Color::Cyan, 2);
            CHECK(!brick.isDestroyed() && brick.getHitsRemaining() == 2);
            CHECK(!brick.hit());
            CHECK(brick.getHitsRemaining() == 1);
            CHECK(brick.hit());
            CHECK(brick.isDestroyed());
            CHECK(!brick.hit());
        }},
        {"serve follows a centered paddle and waits for launch", [] {
            GameSession game;
            CHECK(game.state() == PlayState::Ready);
            CHECK(game.lives() == 3 && game.score() == 0);
            CHECK(near(game.paddle().getPosition().x, SCREEN_WIDTH / 2.f - PADDLE_WIDTH / 2.f));
            game.update(0.1f, 1.f);
            CHECK(game.paddle().getPosition().x > SCREEN_WIDTH / 2.f - PADDLE_WIDTH / 2.f);
            CHECK(near(game.ball().getPosition().x + BALL_RADIUS,
                game.paddle().getPosition().x + PADDLE_WIDTH / 2.f));
            CHECK(near(game.ball().getVelocity().y, 0.f));
            game.advance();
            CHECK(game.state() == PlayState::Playing);
            const auto velocity = game.ball().getVelocity();
            CHECK(near(std::hypot(velocity.x, velocity.y), game.level().ballSpeed));
        }},
        {"paddle stays inside both walls", [] {
            GameSession game;
            for (int i = 0; i < 100; ++i) game.update(0.1f, -1.f);
            CHECK(near(game.paddle().getPosition().x, arena::left));
            for (int i = 0; i < 100; ++i) game.update(0.1f, 1.f);
            CHECK(near(game.paddle().getPosition().x + PADDLE_WIDTH, arena::right));
        }},
        {"wall overlap is corrected once without sticking", [] {
            GameSession game;
            game.advance();
            Access::ball(game).setPosition(arena::left - 4.f, 500.f);
            Access::ball(game).setVelocity(-300.f, -300.f);
            tick(game);
            CHECK(near(game.ball().getPosition().x, arena::left));
            CHECK(game.ball().getVelocity().x > 0);
            tick(game);
            CHECK(game.ball().getPosition().x > arena::left);
            CHECK(game.ball().getVelocity().x > 0);
            Access::ball(game).setPosition(arena::right - BALL_RADIUS, 500.f);
            Access::ball(game).setVelocity(300.f, -300.f);
            tick(game);
            CHECK(game.ball().getPosition().x + 2 * BALL_RADIUS <= arena::right);
            CHECK(game.ball().getVelocity().x < 0);
            Access::ball(game).setPosition(600.f, arena::top - 1.f);
            Access::ball(game).setVelocity(50.f, -300.f);
            tick(game);
            CHECK(game.ball().getPosition().y >= arena::top);
            CHECK(game.ball().getVelocity().y > 0);
        }},
        {"paddle bounce is upward, steerable and preserves speed", [] {
            for (float offset : {-40.f, 0.f, 40.f}) {
                GameSession game;
                game.advance();
                Access::ball(game).setPosition(SCREEN_WIDTH / 2.f + offset - BALL_RADIUS,
                                               arena::paddleY - 2 * BALL_RADIUS - 1.f);
                Access::ball(game).setVelocity(30.f, 400.f);
                tick(game);
                const auto velocity = game.ball().getVelocity();
                CHECK(velocity.y < 0);
                CHECK(std::abs(velocity.x) > 1.f);
                CHECK((offset < 0.f) == (velocity.x < 0.f));
                CHECK(near(std::hypot(velocity.x, velocity.y), game.level().ballSpeed));
                CHECK(game.ball().getPosition().y + 2 * BALL_RADIUS < arena::paddleY);
            }
        }},
        {"upward ball does not bounce off paddle again", [] {
            GameSession game;
            game.advance();
            Access::ball(game).setPosition(630.f, arena::paddleY - 5.f);
            Access::ball(game).setVelocity(30.f, -400.f);
            tick(game);
            CHECK(game.ball().getVelocity().y == -400.f);
        }},
        {"lost life resets serve and paddle while preserving bricks and score", [] {
            GameSession game;
            Access::score(game, 250);
            const int remaining = game.bricks().getBricksRemaining();
            game.update(0.1f, 1.f);
            loseBall(game);
            CHECK(game.state() == PlayState::Ready);
            CHECK(game.lives() == 2 && game.score() == 250);
            CHECK(game.bricks().getBricksRemaining() == remaining);
            CHECK(near(game.paddle().getPosition().x, SCREEN_WIDTH / 2.f - PADDLE_WIDTH / 2.f));
            CHECK(near(game.ball().getPosition().x + BALL_RADIUS, SCREEN_WIDTH / 2.f));
            tick(game, 100);
            CHECK(game.lives() == 2);
        }},
        {"three misses end the game and replay resets the campaign", [] {
            GameSession game;
            for (int i = 0; i < 3; ++i) loseBall(game);
            CHECK(game.state() == PlayState::GameOver && game.lives() == 0);
            tick(game, 100);
            CHECK(game.lives() == 0);
            game.advance();
            CHECK(game.state() == PlayState::Ready && game.lives() == 3);
            CHECK(game.score() == 0 && game.levelIndex() == 0);
        }},
        {"pause freezes physics and restores ready or playing state", [] {
            GameSession game;
            game.pause();
            CHECK(game.state() == PlayState::Paused);
            game.advance();
            CHECK(game.state() == PlayState::Paused);
            game.togglePause();
            CHECK(game.state() == PlayState::Ready);
            game.advance();
            tick(game, 10);
            const auto position = game.ball().getPosition();
            game.pause();
            game.update(10.f, 1.f);
            CHECK(game.ball().getPosition() == position);
            game.pause(); // Repeated focus loss must not overwrite the resume state.
            game.togglePause();
            CHECK(game.state() == PlayState::Playing);
            tick(game);
            CHECK(game.ball().getPosition() != position);
        }},
        {"armored contact scores once and separates the ball", [] {
            GameSession game;
            arrangeBrickHit(game, 2);
            tick(game, 2);
            CHECK(game.score() == 25);
            CHECK(game.bricks().getBricksRemaining() == 1);
            CHECK(Access::bricks(game)[0].getHitsRemaining() == 1);
            CHECK(game.ball().getVelocity().y > 0);
            tick(game, 10);
            CHECK(game.score() == 25);
        }},
        {"only one brick is hit at an adjacent seam", [] {
            GameSession game;
            arrangeBrickHit(game);
            Access::bricks(game).emplace_back(668.f, 200.f, 60.f, 20.f, sf::Color::Red);
            Access::ball(game).setPosition(654.f, 221.f);
            tick(game);
            CHECK(game.score() == 100);
            CHECK(game.bricks().getBricksRemaining() == 1);
            tick(game, 5);
            CHECK(game.score() == 100);
        }},
        {"side collision reflects horizontal direction and separates", [] {
            GameSession game;
            arrangeBrickHit(game, 2);
            Access::ball(game).setPosition(578.f, 200.f);
            Access::ball(game).setVelocity(585.f, 0.f);
            tick(game);
            CHECK(game.ball().getVelocity().x < 0);
            CHECK(game.ball().getPosition().x + 2 * BALL_RADIUS < 600.f);
            CHECK(game.score() == 25);
        }},
        {"a long frame cannot tunnel through a brick", [] {
            GameSession game;
            arrangeBrickHit(game, 2);
            Access::ball(game).setPosition(620.f, 245.f);
            Access::ball(game).setVelocity(0.f, -585.f);
            game.update(0.1f, 0.f);
            CHECK(game.score() == 25);
            CHECK(game.ball().getVelocity().y > 0);
            CHECK(game.ball().getPosition().y > 220.f);
        }},
        {"level clear carries score, rewards a life and loads a new layout", [] {
            GameSession game;
            arrangeBrickHit(game);
            tick(game, 3);
            CHECK(game.state() == PlayState::LevelComplete);
            CHECK(game.score() == 600);
            tick(game, 100);
            CHECK(game.score() == 600); // bonus only once
            game.togglePause();
            CHECK(game.state() == PlayState::LevelComplete);
            game.advance();
            CHECK(game.levelIndex() == 1 && game.lives() == 4);
            CHECK(game.state() == PlayState::Ready && game.score() == 600);
            CHECK(game.bricks().getBricksRemaining() > 0);
        }},
        {"the full campaign terminates with victory and caps lives", [] {
            GameSession game;
            for (std::size_t i = 0; i < campaignLevels().size(); ++i) {
                CHECK(game.levelIndex() == i);
                arrangeBrickHit(game);
                tick(game, 3);
                if (i + 1 < campaignLevels().size()) {
                    CHECK(game.state() == PlayState::LevelComplete);
                    game.advance();
                    CHECK(game.lives() <= 5);
                } else {
                    CHECK(game.state() == PlayState::Won);
                }
            }
            CHECK(game.score() == 3600 && game.lives() == 5);
            tick(game, 100);
            CHECK(game.score() == 3600);
            game.advance();
            CHECK(game.levelIndex() == 0 && game.score() == 0 && game.lives() == 3);
        }},
        {"frame rates produce the same physics trajectory", [] {
            GameSession slow;
            GameSession fast;
            slow.advance();
            fast.advance();
            for (int i = 0; i < 15; ++i) slow.update(arena::step * 8.f, 0.f);
            for (int i = 0; i < 60; ++i) fast.update(arena::step * 2.f, 0.f);
            CHECK(near(slow.ball().getPosition().x, fast.ball().getPosition().x, 2.f));
            CHECK(near(slow.ball().getPosition().y, fast.ball().getPosition().y, 2.f));
        }},
        {"invalid delta and stalls cannot teleport the ball", [] {
            GameSession game;
            game.advance();
            const auto position = game.ball().getPosition();
            game.update(-1.f, 0.f);
            game.update(std::numeric_limits<float>::infinity(), 0.f);
            game.update(std::numeric_limits<float>::quiet_NaN(), 0.f);
            CHECK(game.ball().getPosition() == position);
            game.update(10.f, 0.f);
            CHECK(std::abs(game.ball().getPosition().y - position.y) <= game.level().ballSpeed * 0.101f);
        }}
    };
    int failed = 0;
    for (const auto& [name, test] : tests) {
        try { test(); std::cout << "PASS: " << name << '\n'; }
        catch (const std::exception& error) {
            ++failed;
            std::cerr << "FAIL: " << name << ": " << error.what() << '\n';
        }
    }
    std::cout << std::size(tests) - failed << "/" << std::size(tests) << " tests passed\n";
    return failed == 0 ? 0 : 1;
}
