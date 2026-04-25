#pragma once

#include <stdint.h>

typedef struct
{
    uint8_t playerBallAnimFrame;
    uint8_t playerBallAnimTicks;
    uint16_t playerBallX;
    uint16_t playerBallY;
} GameState;

void InitGameState(GameState *state);
void RunGameTick(GameState *state);