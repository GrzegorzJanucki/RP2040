#ifndef jLabel_h
#define jLabel_h
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "jGraph.h"
#include "jFont.h"

typedef struct{
    point2D_t pos; //top left corner point
    int width ;
    int height ;
    const char* pText;
    font_info_t* pFont;
    bool visible;
} label_t;

label_t* CreateLabel(point2D_t pos, font_info_t* pFont, const char* pText);
label_t* DestroyLabel(label_t* pLabel);

int MoveLabel(label_t* pLabel, point2D_t pos, org_t org);

int ShowLabel(label_t* pLabel);
int HideLabel(label_t* pLabel);

#endif