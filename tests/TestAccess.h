#pragma once
#include "core/include/GameSession.h"

// Deterministic fixtures can place the ball without exposing debug controls in the game.
struct GameSessionTestAccess {
    static Ball& ball(GameSession& game) { return game._ball; }
    static Paddle& paddle(GameSession& game) { return game._paddle; }
    static std::vector<Brick>& bricks(GameSession& game) { return game._bricks.getBricks(); }
    static void level(GameSession& game, std::size_t index) {
        game._levelIndex = index;
        game.loadLevel();
    }
    static void state(GameSession& game, PlayState state) { game._state = state; }
    static void score(GameSession& game, int score) { game._score = score; }
    static void lives(GameSession& game, int lives) { game._lives = lives; }
};
