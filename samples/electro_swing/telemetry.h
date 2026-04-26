#pragma once

#include <stdint.h>

typedef struct
{
    uint32_t total;
    uint32_t simulation;
    uint32_t render;
} FrameTimes;