#ifndef jText_h
#define jText_h
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
    int chBuffLength;
    int nChar;
    char* pChData;
    font_info_t* pFont;
    bool visible;
    just_t just;
} txt_wnd_t;

txt_wnd_t* CreateTxtWnd(point2D_t pos, int width, int height, font_info_t* pFont, int length);
txt_wnd_t* DestroyTxtWnd(txt_wnd_t* pText);

int MoveTextWnd(txt_wnd_t* pText, point2D_t pos, org_t org);

int SetStringDataTxtWnd(txt_wnd_t* pText, char* pString);
int ClearStringDataTxtWnd(txt_wnd_t* pText, char* pString);

int ShowTextWnd(txt_wnd_t* pText);
int HideTextWnd(txt_wnd_t* pText);
int ClearTextWnd(txt_wnd_t* pText);

#endif