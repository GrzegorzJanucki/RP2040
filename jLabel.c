#include "jLabel.h"

void drawPixel(int16_t x, int16_t y, uint8_t colour);

int CalculateLabelWidth(label_t* pLabel){
    int w = 0, dx = 0, i = 0, idx;
    char c;
    while ((c = pLabel->pText[i++]) != '\0')
    {
        idx = GetCharFontIndex(c, pLabel->pFont);
        dx = GetCharFontWidth(idx, pLabel->pFont);
        w += dx;
    };
    return w;
}

label_t* CreateLabel(point2D_t pos, font_info_t* pFont, const char* pText){
    label_t *pLabel = (label_t *)malloc(sizeof(label_t));
    if (pLabel)
    {
        pLabel->pos.x = pos.x;
        pLabel->pos.y = pos.y;
        pLabel->pFont = pFont;
        pLabel->pText = pText;
        pLabel->width = CalculateLabelWidth(pLabel);
        pLabel->height = pFont->height;
        pLabel->visible = false;
    }
    return pLabel;
}

label_t* DestroyLabel(label_t* pLabel){
    if(pLabel)
        free(pLabel);
    return NULL;
}

int MoveLabel(label_t* pLabel, point2D_t pos, org_t org){
    int x = pLabel->pos.x;
    int y = pLabel->pos.y;
    int w = pLabel->width;
    int h = pLabel->height;
    if (!pLabel)
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
    pLabel->pos.x = x;
    pLabel->pos.y = y;
    return 0;
}

int ShowLabel(label_t* pLabel){
    if(!pLabel)
        return -1;
    int w = PrintStringFont(pLabel->pos.x, pLabel->pos.y, (char*)pLabel->pText, pLabel->pFont);
    pLabel->visible = true;
    return 0;
}

int HideLabel(label_t* pLabel){
    if(!pLabel)
        return -1;
    int x = pLabel->pos.x;
    int y = pLabel->pos.y;
    int w = pLabel->width;
    int h = pLabel->height;
    for (int i = 0; i < w; i++)
    {
        y = pLabel->pos.y;
        for (int j = 0; j < h; j++)
        {
            drawPixel(x, y, 0);
            y++;
        }
        x++;
    }
    pLabel->visible = false;
    return 0;
}