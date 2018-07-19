#pragma once

#include <stdio.h>

typedef struct {
    unsigned short height, width;
    unsigned char mask;
    // some sort of map data, probably a bitmap?
} map_t;