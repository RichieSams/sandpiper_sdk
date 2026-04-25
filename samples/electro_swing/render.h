#pragma once

#include "simulation.h"

struct SPPlatform;

void InitRenderState(struct SPPlatform *platform);
void RenderFrame(struct SPPlatform *platform, GameState *state);
