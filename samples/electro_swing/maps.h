#pragma once

#include <stdint.h>

typedef struct
{
    uint16_t posX;
    uint16_t posY;
    uint16_t radius;
} Anchor;

typedef struct
{
    uint16_t goalX0;
    uint16_t goalY0;
    uint16_t goalX1;
    uint16_t goalY1;

    uint16_t startX;
    uint16_t startY;

    const uint8_t *const *tiles;
    uint16_t tileStride;
    uint16_t numTiles;

    uint16_t mapWidth;
    uint16_t mapHeight;

    const Anchor *anchors;
    uint16_t numAnchors;
} Map;

extern const Map map1;
