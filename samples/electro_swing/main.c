#include "assets.h"
#include "simulation.h"
#include "render.h"
#include "clock.h"
#include "telemetry.h"

#include "core.h"
#include "platform.h"
#include "vpu.h"
#include "controller.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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

    FrameTimes frameTimes = {};
    uint64_t lastFrameStart = time_now_ns();
    uint32_t accumulatedFrameTime = 0;
    do
    {
        // Vsync barrier
        // Wait for previous frame (if any) to consume swap command + barrier, then swap buffers
        while (VPUGetFIFONotEmpty(s_platform->vx))
        {
        }
        VPUSwapPages(s_platform->vx, s_platform->sc);

        uint64_t now = time_now_ns();
        uint32_t timeDelta = (uint32_t)(now - lastFrameStart);
        lastFrameStart = now;
        frameTimes.total = timeDelta;

        accumulatedFrameTime += timeDelta;
        while (accumulatedFrameTime > LOGIC_TICK_TIME_NS)
        {
            uint32_t start = time_now_ns();

            accumulatedFrameTime -= LOGIC_TICK_TIME_NS;
            RunGameTick(&gameState);

            frameTimes.simulation = (uint32_t)(time_now_ns() - start);
        }

        // Render
        RenderFrame(s_platform, &gameState, &frameTimes);

        // Queue vsync
        // This will be processed by the VPU asynchronously when the video beam reaches the vertical blanking interval (vblank).
        // It ensures that the buffer swap happens at the correct time to prevent screen tearing.
        VPUSyncSwap(s_platform->vx, 0);
        VPUNoop(s_platform->vx);
    } while (1);

    return 0;
}
