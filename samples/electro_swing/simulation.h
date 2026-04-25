#pragma once

#include <stdint.h>
#include <stdbool.h>

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
    AnimatedSpriteInfo animInfo;
    PlayerBallState state;
    uint16_t posX;
    uint16_t posY;
} PlayerBall;

typedef struct
{
    int controllerValue;
    bool controllerPressed;

    PlayerBall playerBall;
} GameState;

void InitGameState(GameState *state);
void RunGameTick(GameState *state);