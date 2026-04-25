#include "assets.h"
#include "simulation.h"
#include "render.h"
#include "clock.h"

#include "core.h"
#include "platform.h"
#include "vpu.h"
#include "controller.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define LOGIC_TICK_TIME_MS 16
#define LOGIC_TICK_TIME_NS (LOGIC_TICK_TIME_MS * 1000000)

static struct SPPlatform *s_platform = NULL;
GameState gameState;

int main(int argc, char **argv)
{
    s_platform = SPInitPlatform();

    InitRenderState(s_platform);
    InitGameState(&gameState);

    if (controller_open(CONTROLLER_MODE_AUTO) != CONTROLLER_OK)
    {
        printf("Failed to access controller\n");
        exit(-1);
    }

    uint64_t lastFrameStart = time_now_ns();
    uint64_t accumulatedFrameTime = 0;
    do
    {
        // Vsync barrier
        // Wait for previous frame (if any) to consume swap command + barrier, then swap buffers
        while (VPUGetFIFONotEmpty(s_platform->vx))
        {
        }
        VPUSwapPages(s_platform->vx, s_platform->sc);

        uint64_t now = time_now_ns();
        uint64_t timeDelta = now - lastFrameStart;
        lastFrameStart = now;

        accumulatedFrameTime += timeDelta;
        while (accumulatedFrameTime > LOGIC_TICK_TIME_NS)
        {
            accumulatedFrameTime -= LOGIC_TICK_TIME_NS;
            RunGameTick(&gameState);
        }

        // Render
        RenderFrame(s_platform, &gameState);

        // Queue vsync
        // This will be processed by the VPU asynchronously when the video beam reaches the vertical blanking interval (vblank).
        // It ensures that the buffer swap happens at the correct time to prevent screen tearing.
        VPUSyncSwap(s_platform->vx, 0);
        VPUNoop(s_platform->vx);
    } while (1);

    return 0;
}
