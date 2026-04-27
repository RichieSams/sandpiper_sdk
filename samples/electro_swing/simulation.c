#include "simulation.h"

#include "assets.h"
#include "util.h"

#include "controller.h"
#include "vec.h"

#define ORBIT_VELOCITY_INCREASE 0.0003f
#define ORBIT_MAX_VELOCITY 0.08f

void InitGameState(GameState *state)
{
    state->controllerPressed = false;
    state->gameplayPaused = true;

    state->playerBall.animInfo.animTicks = 0;
    state->playerBall.animInfo.currentFrame = 0;
    state->playerBall.state = PLAYER_BALL_ORBIT;
    state->playerBall.orbit.originX = 200;
    state->playerBall.orbit.originY = 200;
    state->playerBall.orbit.radius = 100;
    state->playerBall.orbit.angularVelocity = 0.01f;
    state->playerBall.orbit.theta = 0.0;
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

    if (state->gameplayPaused && state->controllerPressed)
    {
        state->gameplayPaused = false;
    }

    switch (state->playerBall.state)
    {
    case PLAYER_BALL_FREE:
        // TODO: implement
        break;
    case PLAYER_BALL_ORBIT:
        // Increase the velocity while they are still attached
        // But cap it at the maximum
        if (state->playerBall.orbit.angularVelocity < 0)
        {
            state->playerBall.orbit.angularVelocity -= ORBIT_VELOCITY_INCREASE;
            if (state->playerBall.orbit.angularVelocity < -ORBIT_MAX_VELOCITY)
            {
                state->playerBall.orbit.angularVelocity = -ORBIT_MAX_VELOCITY;
            }
        }
        else
        {
            state->playerBall.orbit.angularVelocity += ORBIT_VELOCITY_INCREASE;
            if (state->playerBall.orbit.angularVelocity > ORBIT_MAX_VELOCITY)
            {
                state->playerBall.orbit.angularVelocity = ORBIT_MAX_VELOCITY;
            }
        }

        // Calculate the new position
        state->playerBall.orbit.theta += state->playerBall.orbit.angularVelocity;
        // Clamp to [0, 2PI)
        while (state->playerBall.orbit.theta < 0.0f)
        {
            state->playerBall.orbit.theta += (PI + PI);
        }
        while (state->playerBall.orbit.theta >= (PI + PI))
        {
            state->playerBall.orbit.theta -= (PI + PI);
        }

        // Convert from angular coordinates to cartesian
        state->playerBall.posX = state->playerBall.orbit.originX + (state->playerBall.orbit.radius * cos(state->playerBall.orbit.theta));
        state->playerBall.posY = state->playerBall.orbit.originY + (state->playerBall.orbit.radius * sin(state->playerBall.orbit.theta));
        break;
    }

    // Update the player ball animation
    ++state->playerBall.animInfo.animTicks;
    if (state->playerBall.animInfo.animTicks >= WATER_BALL_ANIMATION_TICK_RATE)
    {
        state->playerBall.animInfo.animTicks = 0;
        ++state->playerBall.animInfo.currentFrame;
        if (state->playerBall.animInfo.currentFrame >= WATER_BALL_FRAME_COUNT)
        {
            state->playerBall.animInfo.currentFrame = 0;
        }
    }
}