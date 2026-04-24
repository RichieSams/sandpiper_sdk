#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "assets.h"

#include "core.h"
#include "platform.h"
#include "vpu.h"
#include "vec.h"

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#define VIDEO_MODE EVM_640_480
#define VIDEO_COLOR ECM_8bit_Indexed
#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480

#define LOGIC_TICK_TIME_MS 16
#define LOGIC_TICK_TIME_NS (LOGIC_TICK_TIME_MS * 1000000)
#define ANIMATION_TICK_RATE 8

static struct SPPlatform *s_platform = NULL;
struct SPSizeAlloc frameBufferA;
struct SPSizeAlloc frameBufferB;

static void masked_blit_8(uint8_t *dst, uint32_t dst_stride, int dst_w, int dst_h, const uint8_t *src, int src_w, int src_h, int dst_x, int dst_y, uint8_t key)
{
    int src_x = 0;
    int src_y = 0;
    int w = src_w;
    int h = src_h;

    // Clip the source and destination rectangles to ensure we don't read/write out of screen bounds
    if (dst_x < 0)
    {
        src_x = -dst_x;
        dst_x = 0;
        w -= src_x;
    }
    if (dst_y < 0)
    {
        src_y = -dst_y;
        dst_y = 0;
        h -= src_y;
    }
    if (dst_x + w > dst_w)
        w = dst_w - dst_x;
    if (dst_y + h > dst_h)
        h = dst_h - dst_y;
    if (w <= 0 || h <= 0)
        return;

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
    uint8x16_t keyv = vdupq_n_u8(key); // Broadcast the key to 16 bytes
#endif

    for (int y = 0; y < h; ++y)
    {
        uint8_t *d = dst + (uint32_t)(dst_y + y) * dst_stride + dst_x;
        const uint8_t *s = src + (src_y + y) * src_w + src_x;
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

static uint64_t monotonic_ns()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}

typedef struct
{
    uint8_t playerBallAnimFrame;
    uint8_t playerBallAnimTicks;
    uint16_t playerBallX;
    uint16_t playerBallY;
} GameState;

GameState gameState;

int main(int argc, char **argv)
{
    s_platform = SPInitPlatform();

    // Set up the video output mode
    VPUSetVideoMode(s_platform->vx, VIDEO_MODE, VIDEO_COLOR, EVS_Enable);

    // Set the palette
    for (uint8_t i = 0; i < 255; ++i)
    {
        VPUSetPal32(s_platform->vx, i, colorPalette[i]);
    }

    // Allocate our two frame buffers
    uint32_t stride = VPUGetStride(VIDEO_MODE, VIDEO_COLOR);
    frameBufferB.size = frameBufferA.size = stride * VIDEO_HEIGHT;
    SPAllocateBuffer(s_platform, &frameBufferA);
    SPAllocateBuffer(s_platform, &frameBufferB);

    // For hardware assisted vsync, we need to set this up once at start
    VPUSetScanoutAddress(s_platform->vx, (uint32_t)frameBufferA.dmaAddress);
    VPUSetScanoutAddress2(s_platform->vx, (uint32_t)frameBufferB.dmaAddress);
    // This one is for the CPU side so it can keep up with the hardware flips
    s_platform->sc->cycle = 0;
    s_platform->sc->framebufferA = &frameBufferA;
    s_platform->sc->framebufferB = &frameBufferB;

    gameState.playerBallAnimTicks = 0;
    gameState.playerBallAnimFrame = 0;
    gameState.playerBallX = 50;
    gameState.playerBallY = 50;

    uint32_t frame = 0;

    char buffer[128];
    uint64_t lastFrameStart = monotonic_ns();
    uint64_t accumulatedFrameTime = 0;
    do
    {
        // Vsync barrier
        // Wait for previous frame (if any) to consume swap command + barrier, then swap buffers
        while (VPUGetFIFONotEmpty(s_platform->vx))
        {
        }
        VPUSwapPages(s_platform->vx, s_platform->sc);

        uint64_t now = monotonic_ns();
        uint64_t timeDelta = now - lastFrameStart;
        lastFrameStart = now;

        accumulatedFrameTime += timeDelta;
        while (accumulatedFrameTime > LOGIC_TICK_TIME_NS)
        {
            accumulatedFrameTime -= LOGIC_TICK_TIME_NS;

            ++gameState.playerBallAnimTicks;
            if (gameState.playerBallAnimTicks >= ANIMATION_TICK_RATE)
            {
                gameState.playerBallAnimTicks = 0;
                ++gameState.playerBallAnimFrame;
                if (gameState.playerBallAnimFrame >= WATER_BALL_64_FRAME_COUNT)
                {
                    gameState.playerBallAnimFrame = 0;
                }
            }
        }

        {
            ++frame;

            VPUClear(s_platform->vx, 0x0);

            uint8_t *dst = (uint8_t *)s_platform->sc->writepage;

            masked_blit_8(dst, stride, VIDEO_WIDTH, VIDEO_HEIGHT, waterBall64Sprite[gameState.playerBallAnimFrame], WATER_BALL_64_W, WATER_BALL_64_H, 100, 100, 0x00);

            int len = snprintf(buffer, sizeof(buffer), "%d", frame);
            VPUPrintString(s_platform->vx, 1, 0, 5, 5, buffer, len);
        }

        // Queue vsync
        // This will be processed by the VPU asynchronously when the video beam reaches the vertical blanking interval (vblank).
        // It ensures that the buffer swap happens at the correct time to prevent screen tearing.
        VPUSyncSwap(s_platform->vx, 0);
        VPUNoop(s_platform->vx);
    } while (1);

    printf("Done\n");
    return 0;
}
