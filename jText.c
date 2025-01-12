#include "jText.h"
#include <string.h>

void drawPixel(int16_t x, int16_t y, uint8_t colour);

int CalculateTxtStringWidth(txt_wnd_t* pText){
    int w = 0, dx = 0, i = 0, idx;
    char c;
    while ((c = pText->pChData[i++]) != '\0')
    {
        idx = GetCharFontIndex(c, pText->pFont);
        dx = GetCharFontWidth(idx, pText->pFont);
        w += dx;
    };
    return w;
}

txt_wnd_t* CreateTxtWnd(point2D_t pos, int width, int height, font_info_t* pFont, int len){
    txt_wnd_t* ptxt = (txt_wnd_t*)malloc(sizeof(txt_wnd_t));
    if(ptxt){
        ptxt->pChData = (char*)malloc((len+1)*sizeof(char));
            ptxt->pChData[len] = '\0';
        if(!ptxt->pChData){
            free(ptxt);
            ptxt = NULL;
        } else {
            ptxt->width = width;
            ptxt->height = height;
            ptxt->pFont = pFont;
            ptxt->chBuffLength = len;
            ptxt->pos.x = pos.x;
            ptxt->pos.y = pos.y;
            ptxt->visible = false;
            ptxt->just = left;
        }
    }
    return ptxt;
}

txt_wnd_t* DestroyTxtWnd(txt_wnd_t* pText){
    if(pText){
        if(pText->pChData)
            free(pText->pChData);
        free(pText);    
    }
    return NULL;
}

int MoveTextWnd(txt_wnd_t* pText, point2D_t pos, org_t org){
    int x = pText->pos.x;
    int y = pText->pos.y;
    int w = pText->width;
    int h = pText->height;
    if (!pText)
        return -1;
    switch (org)
    {
    case topleft:
        x = pos.x;
        y = pos.y;
        break;
    case topright:
        x = pos.x - w;
        y = pos.y;
        break;
    case botright:
        x = pos.x - w;
        y = pos.y - h;
        break;
    case botleft:
        x = pos.x;
        y = pos.y - h;
        break;
    case midleft:
        x = pos.x;
        y = pos.y - h/2;
        break;
    case midright:
        x = pos.x - w;
        y = pos.y - h/2;
        break;
    case topmid:
        x = pos.x - w/2;
        y = pos.y;
        break;
    case botmid:
        x = pos.x - w/2;
        y = pos.y - h;
        break;
    case center:
        x = pos.x - w/2;
        y = pos.y - h/2;
        break;
    default:
        break;
    }
    pText->pos.x = x;
    pText->pos.y = y;
    return 0;
}

int SetStringDataTxtWnd(txt_wnd_t* pText, char* pString){   
    strncpy(pText->pChData, pString, pText->chBuffLength);
    pText->nChar = strlen(pText->pChData);
    return 0;
}
int ClearStringDataTxtWnd(txt_wnd_t* pText, char* pString){
    pText->pChData[0] = '\0';
    pText->nChar = 0;
    return 0;
}

int ShowTextWnd(txt_wnd_t* pText){
    if(!pText)
        return -1;
    int w = CalculateTxtStringWidth(pText);
    int xl, yt, xr, yb;
    if(pText->just == right){
        xl = pText->pos.x + pText->width - w;
        xr = pText->pos.x + pText->width;
        yt = pText->pos.y;
        yb = pText->pos.y + pText->height;
    } else {
        xl = pText->pos.x;
        xr = pText->pos.x + pText->width;
        yt = pText->pos.y;
        yb = pText->pos.y + pText->height;
    }
    PrintStringFontClip(xl, yt, xr, yb, (char*)pText->pChData, pText->pFont);
    pText->visible = true;
    return 0;
}
int HideTextWnd(txt_wnd_t* pText){
    if(!pText)
        return -1;
    int x = pText->pos.x;
    int y = pText->pos.y;
    int w = pText->width;
    int h = pText->height;
    for (int i = 0; i < w; i++)
    {
        y = pText->pos.y;
        for (int j = 0; j < h; j++)
        {
            drawPixel(x, y, 0);
            y++;
        }
        x++;
    }
    pText->visible = false;
    return 0;
}

int ClearTextWnd(txt_wnd_t* pText){
    int xs = pText->pos.x;
    int ys = pText->pos.y;
    int w = pText->width;
    int h = pText->height;
    int xe = xs + w;
    int ye = ys + ye;
    int x = xs, y = ys;
    for (int i = 0; i < w; i++)
    {
        y = ys;
        for (int j = 0; j < h; j++)
        {
            if((x >= xs)&&(y >= ys)&&(x<=xe)&&(y<=ye))
                drawPixel(x, y, 0);
            y++;
        }
        x++;
    }
    return 0;
}