#include "simulation.h"

#include "assets.h"

#include "controller.h"

void InitGameState(GameState *state)
{
    state->playerBall.animInfo.animTicks = 0;
    state->playerBall.animInfo.currentFrame = 0;
    state->playerBall.posX = 50;
    state->playerBall.posY = 50;
}

void RunGameTick(GameState *state)
{
    int r = controller_read(&state->controllerValue);

    if (state->controllerPressed)
    {
        // If we detect a falling edge, treat it as off
        if (state->controllerValue <= 90)
        {
            state->controllerPressed = false;
        }
    }
    else
    {
        // If we detect a rising edge, treat it as on
        if (state->controllerValue >= 120)
        {
            state->controllerPressed = true;
        }
    }

    // Update the player ball animation
    ++state->playerBall.animInfo.animTicks;
    if (state->playerBall.animInfo.animTicks >= WATER_BALL_64_ANIMATION_TICK_RATE)
    {
        state->playerBall.animInfo.animTicks = 0;
        ++state->playerBall.animInfo.currentFrame;
        if (state->playerBall.animInfo.currentFrame >= WATER_BALL_64_FRAME_COUNT)
        {
            state->playerBall.animInfo.currentFrame = 0;
        }
    }
}