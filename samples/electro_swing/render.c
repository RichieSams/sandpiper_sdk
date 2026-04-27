#include "render.h"

#include "assets.h"
#include "clock.h"
#include "util.h"

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#include "vpu.h"

#include <stdio.h>
#include <stdlib.h>

#define VIDEO_MODE EVM_640_480
#define VIDEO_COLOR ECM_8bit_Indexed
#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480

uint32_t frameStride;
struct SPSizeAlloc frameBufferA;
struct SPSizeAlloc frameBufferB;

static void maskedBlit8(uint8_t *dest, uint32_t destStride, int destW, int destH, const uint8_t *src, int srcW, int srcH, int destX, int destY, uint8_t key);
static void drawCircle(uint8_t *fb, uint32_t stride, int cx, int cy, int radius, uint8_t color);
static void drawLine(uint8_t *fb, uint32_t stride, int x0, int y0, int x1, int y1, uint8_t color);

void InitRenderState(struct SPPlatform *platform)
{
    // Set up the video output mode
    VPUSetVideoMode(platform->vx, VIDEO_MODE, VIDEO_COLOR, EVS_Enable);

    // Set the palette
    for (uint8_t i = 0; i < 255; ++i)
    {
        VPUSetPal32(platform->vx, i, colorPalette[i]);
    }

    // Allocate our two frame buffers
    frameStride = VPUGetStride(VIDEO_MODE, VIDEO_COLOR);
    frameBufferB.size = frameBufferA.size = frameStride * VIDEO_HEIGHT;
    SPAllocateBuffer(platform, &frameBufferA);
    SPAllocateBuffer(platform, &frameBufferB);

    // For hardware assisted vsync, we need to set this up once at start
    VPUSetScanoutAddress(platform->vx, (uint32_t)frameBufferA.dmaAddress);
    VPUSetScanoutAddress2(platform->vx, (uint32_t)frameBufferB.dmaAddress);
    // This one is for the CPU side so it can keep up with the hardware flips
    platform->sc->cycle = 0;
    platform->sc->framebufferA = &frameBufferA;
    platform->sc->framebufferB = &frameBufferB;
}

void RenderFrame(struct SPPlatform *platform, GameState *state, FrameTimes *frameTimes)
{
    uint64_t start = time_now_ns();

    VPUClear(platform->vx, 0x0);

    uint8_t *dest = (uint8_t *)platform->sc->writepage;

    // Render the map
    // TODO: Implement for real
    drawCircle(dest, frameStride, 200, 200, 20, 1);

    // Render the player

    // Pick the sprite based on the direction they're going
    const uint8_t *sprite;
    switch (state->playerBall.state)
    {
    case PLAYER_BALL_FREE:
        // TODO: Do the math
        sprite = waterBall000Sprite[state->playerBall.animInfo.currentFrame];
        break;
    case PLAYER_BALL_ORBIT:
        if ((state->playerBall.orbit.theta >= (PI + PI_DIV_2 + PI_DIV_4 + PI_DIV_8) && state->playerBall.orbit.theta < (PI + PI)) || (state->playerBall.orbit.theta >= 0 && state->playerBall.orbit.theta < PI_DIV_8))
        {
            // East
            if (state->playerBall.orbit.angularVelocity > 0)
            {
                // Clockwise
                sprite = waterBall180Sprite[state->playerBall.animInfo.currentFrame];
            }
            else
            {
                // Counter-clockwise
                sprite = waterBall000Sprite[state->playerBall.animInfo.currentFrame];
            }
        }
        else if (state->playerBall.orbit.theta >= PI_DIV_8 && state->playerBall.orbit.theta < (PI_DIV_4 + PI_DIV_8))
        {
            // South-East
            if (state->playerBall.orbit.angularVelocity > 0)
            {
                // Clockwise
                sprite = waterBall225Sprite[state->playerBall.animInfo.currentFrame];
            }
            else
            {
                // Counter-clockwise
                sprite = waterBall045Sprite[state->playerBall.animInfo.currentFrame];
            }
        }
        else if (state->playerBall.orbit.theta >= (PI_DIV_4 + PI_DIV_8) && state->playerBall.orbit.theta < (PI_DIV_2 + PI_DIV_8))
        {
            // South
            if (state->playerBall.orbit.angularVelocity > 0)
            {
                // Clockwise
                sprite = waterBall270Sprite[state->playerBall.animInfo.currentFrame];
            }
            else
            {
                // Counter-clockwise
                sprite = waterBall090Sprite[state->playerBall.animInfo.currentFrame];
            }
        }
        else if (state->playerBall.orbit.theta >= (PI_DIV_2 + PI_DIV_8) && state->playerBall.orbit.theta < (PI_DIV_2 + PI_DIV_4 + PI_DIV_8))
        {
            // South-West
            if (state->playerBall.orbit.angularVelocity > 0)
            {
                // Clockwise
                sprite = waterBall315Sprite[state->playerBall.animInfo.currentFrame];
            }
            else
            {
                // Counter-clockwise
                sprite = waterBall135Sprite[state->playerBall.animInfo.currentFrame];
            }
        }
        else if (state->playerBall.orbit.theta >= (PI_DIV_2 + PI_DIV_4 + PI_DIV_8) && state->playerBall.orbit.theta < (PI + PI_DIV_8))
        {
            // West
            if (state->playerBall.orbit.angularVelocity > 0)
            {
                // Clockwise
                sprite = waterBall000Sprite[state->playerBall.animInfo.currentFrame];
            }
            else
            {
                // Counter-clockwise
                sprite = waterBall180Sprite[state->playerBall.animInfo.currentFrame];
            }
        }
        else if (state->playerBall.orbit.theta >= (PI + PI_DIV_8) && state->playerBall.orbit.theta < (PI + PI_DIV_4 + PI_DIV_8))
        {
            // North-West
            if (state->playerBall.orbit.angularVelocity > 0)
            {
                // Clockwise
                sprite = waterBall045Sprite[state->playerBall.animInfo.currentFrame];
            }
            else
            {
                // Counter-clockwise
                sprite = waterBall225Sprite[state->playerBall.animInfo.currentFrame];
            }
        }
        else if (state->playerBall.orbit.theta >= (PI + PI_DIV_4 + PI_DIV_8) && state->playerBall.orbit.theta < (PI + PI_DIV_2 + PI_DIV_8))
        {
            // North
            if (state->playerBall.orbit.angularVelocity > 0)
            {
                // Clockwise
                sprite = waterBall090Sprite[state->playerBall.animInfo.currentFrame];
            }
            else
            {
                // Counter-clockwise
                sprite = waterBall270Sprite[state->playerBall.animInfo.currentFrame];
            }
        }
        else if (state->playerBall.orbit.theta >= (PI + PI_DIV_2 + PI_DIV_8) && state->playerBall.orbit.theta < (PI + PI_DIV_2 + PI_DIV_4 + PI_DIV_8))
        {
            // North-East
            if (state->playerBall.orbit.angularVelocity > 0)
            {
                // Clockwise
                sprite = waterBall135Sprite[state->playerBall.animInfo.currentFrame];
            }
            else
            {
                // Counter-clockwise
                sprite = waterBall315Sprite[state->playerBall.animInfo.currentFrame];
            }
        }

        // Render the lasso between the player and the anchor
        drawLine(dest, frameStride, state->playerBall.orbit.originX, state->playerBall.orbit.originY, state->playerBall.posX, state->playerBall.posY, 7);

        break;
    }
    maskedBlit8(dest, frameStride, VIDEO_WIDTH, VIDEO_HEIGHT, sprite, WATER_BALL_W, WATER_BALL_H, state->playerBall.posX - (WATER_BALL_W / 2), state->playerBall.posY - (WATER_BALL_H / 2), 0x00);

    frameTimes->render = (uint32_t)(time_now_ns() - start);

    // Render the frame times
    char buffer[32];
    int len = snprintf(buffer, sizeof(buffer), "Total: %0.2fms", (float)frameTimes->total / 1000000.0f);
    VPUPrintString(platform->vx, 1, 0, 1, 1, buffer, len);
    len = snprintf(buffer, sizeof(buffer), "Simulation: %0.2fms", (float)frameTimes->simulation / 1000000.0f);
    VPUPrintString(platform->vx, 1, 0, 1, 3, buffer, len);
    len = snprintf(buffer, sizeof(buffer), "Render: %0.2fms", (float)frameTimes->render / 1000000.0f);
    VPUPrintString(platform->vx, 1, 0, 1, 5, buffer, len);
}

static void maskedBlit8(uint8_t *dest, uint32_t destStride, int destW, int destH, const uint8_t *src, int srcW, int srcH, int destX, int destY, uint8_t key)
{
    int srcX = 0;
    int srcY = 0;
    int w = srcW;
    int h = srcH;

    // Clip the source and destination rectangles to ensure we don't read/write out of screen bounds
    if (destX < 0)
    {
        srcX = -destX;
        destX = 0;
        w -= srcX;
    }
    if (destY < 0)
    {
        srcY = -destY;
        destY = 0;
        h -= srcY;
    }
    if (destX + w > destW)
        w = destW - destX;
    if (destY + h > destH)
        h = destH - destY;
    if (w <= 0 || h <= 0)
        return;

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
    uint8x16_t keyv = vdupq_n_u8(key); // Broadcast the key to 16 bytes
#endif

    for (int y = 0; y < h; ++y)
    {
        uint8_t *d = dest + (uint32_t)(destY + y) * destStride + destX;
        const uint8_t *s = src + (srcY + y) * srcW + srcX;
        int x = 0;

        // If NEON is available, we can process 16 pixels at a time.
        // The key is compared against the source pixels, and if it matches,
        // the destination pixel is kept; otherwise, the source pixel is copied to the destination.
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
        for (; x + 15 < w; x += 16)
        {
            uint8x16_t sv = vld1q_u8(s + x);         // Load source pixels
            uint8x16_t dv = vld1q_u8(d + x);         // Load destination pixels
            uint8x16_t mask = vceqq_u8(sv, keyv);    // Compare source pixels with key, result is 0xFF where equal, 0x00 where not
            uint8x16_t out = vbslq_u8(mask, dv, sv); // If mask bit is 1, select from dv (keep dest), else select from sv (copy src)
            vst1q_u8(d + x, out);                    // Store result back to destination
        }
#endif
        // Process any remaining pixels that don't fit into a 16-byte block
        for (; x < w; ++x)
        {
            uint8_t px = s[x];
            if (px != key)
                d[x] = px;
        }
    }
}

static inline void setPixel(uint8_t *fb, uint32_t stride, int x, int y, uint8_t color)
{
    fb[y * stride + x] = color;
}

// Draw a circle using midpoint algorithm
static void drawCircle(uint8_t *fb, uint32_t stride, int cx, int cy, int radius, uint8_t color)
{
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y)
    {
        setPixel(fb, stride, cx + x, cy + y, color);
        setPixel(fb, stride, cx + y, cy + x, color);
        setPixel(fb, stride, cx - y, cy + x, color);
        setPixel(fb, stride, cx - x, cy + y, color);
        setPixel(fb, stride, cx - x, cy - y, color);
        setPixel(fb, stride, cx - y, cy - x, color);
        setPixel(fb, stride, cx + y, cy - x, color);
        setPixel(fb, stride, cx + x, cy - y, color);

        y++;
        if (err <= 0)
        {
            err += 2 * y + 1;
        }
        if (err > 0)
        {
            x--;
            err -= 2 * x + 1;
        }
    }
}

static void drawLine(uint8_t *fb, uint32_t stride, int x0, int y0, int x1, int y1, uint8_t color)
{
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (true)
    {
        setPixel(fb, stride, x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;
        int e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}