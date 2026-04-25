#include "simulation.h"

#include "assets.h"

void InitGameState(GameState *state)
{
    state->playerBallAnimTicks = 0;
    state->playerBallAnimFrame = 0;
    state->playerBallX = 50;
    state->playerBallY = 50;
}

void RunGameTick(GameState *state)
{
    ++state->playerBallAnimTicks;
    if (state->playerBallAnimTicks >= WATER_BALL_64_ANIMATION_TICK_RATE)
    {
        state->playerBallAnimTicks = 0;
        ++state->playerBallAnimFrame;
        if (state->playerBallAnimFrame >= WATER_BALL_64_FRAME_COUNT)
        {
            state->playerBallAnimFrame = 0;
        }
    }
}