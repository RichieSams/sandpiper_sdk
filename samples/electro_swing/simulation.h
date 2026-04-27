#pragma once

#include <stdint.h>
#include <stdbool.h>

#define LOGIC_TICK_TIME_MS 16
#define LOGIC_TICK_TIME_NS (LOGIC_TICK_TIME_MS * 1000000)

typedef struct
{
    int currentFrame;
    int animTicks;
} AnimatedSpriteInfo;

typedef enum
{
    PLAYER_BALL_FREE,
    PLAYER_BALL_ORBIT,
} PlayerBallState;

typedef struct
{
    // This velocity is per *tick*. Not per second
    // This is fine, since we have fixed size ticks. And this makes the math way easier
    uint16_t x;
    uint16_t y;
} FreeVelocity;

typedef struct
{
    uint16_t originX;
    uint16_t originY;
    float theta;
    float radius;
    // This velocity is per *tick*. Not per second
    // This is fine, since we have fixed size ticks. And this makes the math way easier
    float angularVelocity;
} OrbitVelocity;

typedef struct
{
    AnimatedSpriteInfo animInfo;
    PlayerBallState state;
    union
    {
        FreeVelocity free;
        OrbitVelocity orbit;
    };

    uint16_t posX;
    uint16_t posY;
} PlayerBall;

typedef struct
{
    int controllerValue;
    bool controllerPressed;

    bool gameplayPaused;

    PlayerBall playerBall;
} GameState;

void InitGameState(GameState *state);
void RunGameTick(GameState *state);