#ifndef jGraph_h
#define jGraph_h
#pragma once

#include <stdint.h>
#include <stdio.h>

typedef enum {
    topleft,
    topright,
    botright,
    botleft,
    midleft,
    midright,
    topmid,
    botmid,
    center
} org_t;

typedef enum {
    left,
    right
} just_t;

typedef struct{
    int x;
    int y;
} point2D_t;

#endif