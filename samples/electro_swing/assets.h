#pragma once

#include <stdint.h>

extern const uint32_t colorPalette[256];

#define WATER_BALL_64_W 64
#define WATER_BALL_64_H 64
#define WATER_BALL_64_FRAME_COUNT 12
#define WATER_BALL_64_ANIMATION_TICK_RATE 8

extern const uint8_t *const waterBall64Sprite[WATER_BALL_64_FRAME_COUNT];
