#pragma once

#include "simulation.h"
#include "telemetry.h"

struct SPPlatform;

void InitRenderState(struct SPPlatform *platform);
void RenderFrame(struct SPPlatform *platform, GameState *state, FrameTimes *frameTimes);
