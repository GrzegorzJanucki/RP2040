#include <math.h> // for abs function
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef ch1115_graphics_h
#define ch1115_graphics_h
#pragma once


	uint8_t writeChar(int16_t x, int16_t y, char value);
	uint8_t setFont(const uint8_t *SelectedFontName);
	void setCursor(int16_t x, int16_t y);
	void CH1115_graphics_init(void);


	uint8_t setFont(const uint8_t *font);
	void setInvertFont(bool invertStatus);
	bool getInvertFont(void);
	size_t write(uint8_t character);

	extern const uint8_t *_FontSelect; /**< Pointer to the active font,  Fonts Stored are Const */

	extern uint8_t _Font_X_Size;  /**< Width Size of a Font character */
	extern uint8_t _Font_Y_Size;  /**< Height Size of a Font character */
	extern uint8_t _FontOffset;	  /**< Offset in the ASCII table 0x00 to 0xFF, where font begins */
	extern uint8_t _FontNumChars; /**< Number of characters in font 0x00 to 0xFE */


    #define WIDTH 128;  /**< This is the 'raw' display w - never changes */
	#define HEIGHT 64; /**< This is the 'raw' display h - never changes*/
	extern int16_t _width;		  /**< Display w as modified by current rotation*/
	extern int16_t _height;	  /**< Display h as modified by current rotation*/
	extern int16_t _cursor_x;	  /**< Current X co-ord cursor position */
	extern int16_t _cursor_y;	  /**< Current Y co-ord cursor position */
    extern bool _FontInverted; /**< Is the font inverted , False = normal , true = inverted*/
    extern bool _textwrap;		  /**< If set, 'wrap' text at right edge of display*/
	extern bool _drawBitmapAddr; /**< data addressing mode for method drawBitmap, True-vertical , false-horizontal */

#endif