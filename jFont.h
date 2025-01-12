#ifndef jFont_h
#define jFont_h
#pragma once

#include <stdint.h>
#include <stdio.h>

typedef  struct{
    int stride;
    int width;
    int height;
    const uint8_t* pWidths;
    const uint8_t* pData;
    const uint8_t* pCharset;
} font_info_t;	


int GetCharFontIndex(char ch, font_info_t* pFontInfo);
int GetCharFontWidth(int index, font_info_t* pFontInfo);
uint8_t* GetCharFontData(int index, font_info_t* pFontInfo);

int16_t WriteCharFont(int16_t x, int16_t y, char value, font_info_t* pFontInfo);
int16_t PrintStringFont(int16_t x_left, int16_t y_top, char* pString, font_info_t* pFontInfo);
int16_t WriteCharFontClip(int16_t x, int16_t y, int16_t x_right, int16_t y_bottom, char value, font_info_t* pFontInfo);
int16_t PrintStringFontClip(int16_t x_left, int16_t y_top, int16_t x_right, int16_t y_bottom, 
                            char* pString, font_info_t* pFontInfo);

extern font_info_t Font7SegsInfo;
extern font_info_t FontDigital32Info;
extern font_info_t FontDigital18Info;

#endif