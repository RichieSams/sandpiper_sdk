#include "maps.h"

#include "assets.h"
#include "util.h"
static const uint8_t *const map1Tiles[] = {
    wallTile, wallTile, wallTile, wallTile, wallTile, wallTile, wallTile, wallTile, wallTile, wallTile, wallTile, wallTile, wallTile, wallTile, wallTile, wallTile, wallTile, wallTile, wallTile, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, goalTopLeftTile, goalTopRightTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, goalBottomLeftTile, goalBottomRightTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, anchorTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, anchorTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, anchorTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, anchorTile, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, anchorTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, anchorTile, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, anchorTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
    wallTile, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, waterTileVertical, wallTile, 
};

static const Anchor map1Anchors[] = {
    {
        .posX = 208,
        .posY = 336,
        .radius = 100,
    },
    {
        .posX = 432,
        .posY = 336,
        .radius = 100,
    },
    {
        .posX = 176,
        .posY = 656,
        .radius = 100,
    },
    {
        .posX = 496,
        .posY = 656,
        .radius = 100,
    },
    {
        .posX = 176,
        .posY = 976,
        .radius = 100,
    },
    {
        .posX = 496,
        .posY = 976,
        .radius = 100,
    },
    {
        .posX = 304,
        .posY = 1296,
        .radius = 100,
    },
};


const Map map1 = {
    .goalX0 = 288,
    .goalY0 = 32,
    .goalX1 = 352,
    .goalY1 = 96,

    .startX = (10 * 32) + 16,
    .startY = (42 * 32) + 16,

    .tiles = map1Tiles,
    .tileStride = 20,
    .numTiles = 900,

    .mapWidth = 640,
    .mapHeight = 1440,

    .anchors = map1Anchors,
    .numAnchors = 7,
};
