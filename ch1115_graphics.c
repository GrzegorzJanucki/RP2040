#include "ch1115_graphics.h"
#include "ch1115.h"

#include "ch1115_print.h"
#include "ch1115_font.h"

const uint8_t *_FontSelect; /**< Pointer to the active font,  Fonts Stored are Const */

uint8_t _Font_X_Size;  /**< Width Size of a Font character */
uint8_t _Font_Y_Size;  /**< Height Size of a Font character */
uint8_t _FontOffset;	  /**< Offset in the ASCII table 0x00 to 0xFF, where font begins */
uint8_t _FontNumChars; /**< Number of characters in font 0x00 to 0xFE */


int16_t _width = 128;		  /**< Display w as modified by current rotation*/
int16_t _height = 64;	  /**< Display h as modified by current rotation*/
int16_t _cursor_x;	  /**< Current X co-ord cursor position */
int16_t _cursor_y;	  /**< Current Y co-ord cursor position */
bool _FontInverted; /**< Is the font inverted , False = normal , true = inverted*/
bool _textwrap;		  /**< If set, 'wrap' text at right edge of display*/
bool _drawBitmapAddr;


uint8_t setFont(const uint8_t *SelectedFontName)
{
	if (SelectedFontName == NULL)
	{
		printf("ERMCH1115_OLEDFonts::setFont ERROR 2: Invalid pointer object\r\n");
		return 2;
	}
	_FontSelect = SelectedFontName;
	_Font_X_Size = *(SelectedFontName + 0);
	_Font_Y_Size = *(SelectedFontName + 1);
	_FontOffset = *(SelectedFontName + 2);
	_FontNumChars = *(SelectedFontName + 3);
	_FontInverted = false;
	return 0;
}

void setInvertFont(bool invertStatus)
{
	_FontInverted = invertStatus;
}


bool getInvertFont()
{
	return _FontInverted;
}




void CH1115_graphics_init(void)
{
	_FontSelect = pFontDefault;
	_width = WIDTH;
	_height = HEIGHT;
	_cursor_y = 0;
	_cursor_x = 0;
	_textwrap = true;
	_drawBitmapAddr = true;
	_Font_X_Size = 0x06;  
	_Font_Y_Size = 0x08; 
	_FontOffset = 0x00;	 
	_FontNumChars = 0xFE;
	_FontInverted = false;
}



void setCursor(int16_t x, int16_t y)
{
	_cursor_x = x;
	_cursor_y = y;
}



uint8_t writeChar(int16_t x, int16_t y, char value)
{
	uint16_t fontIndex = 0;
	uint16_t rowCount = 0;
	uint16_t count = 0;
	uint8_t colIndex;
	uint16_t temp = 0;
	int16_t colByte, cx, cy;
	int16_t colbit;

	// 1. Check for screen out of  bounds
	if ((x >= _width) ||				// Clip right
		(y >= _height) ||				// Clip bottom
		((x + _Font_X_Size + 1) < 0) || // Clip left
		((y + _Font_Y_Size) < 0))		// Clip top
	{
		printf("ERMCH1115_graphics::writeChar Error 2: Co-ordinates out of bounds \r\n");
		return 2;
	}
	// 2. Check for character out of font range bounds
	if (value < _FontOffset || value >= (_FontOffset + _FontNumChars + 1))
	{
		printf("ERMCH1115_graphics::writeChar Error 3: Character out of Font bounds  %u : %u<->%u \r\n", value, _FontOffset, _FontOffset + _FontNumChars);
		return 3;
	}
	if (_Font_Y_Size % 8 == 0) // Is the font height divisible by 8
	{
		fontIndex = ((value - _FontOffset) * (_Font_X_Size * (_Font_Y_Size / 8))) + 4;
		for (rowCount = 0; rowCount < (_Font_Y_Size / 8); rowCount++)
		{
			for (count = 0; count < _Font_X_Size; count++)
			{
				temp = *(_FontSelect + fontIndex + count + (rowCount * _Font_X_Size));
				for (colIndex = 0; colIndex < 8; colIndex++)
				{
					if (temp & (1 << colIndex))
					{
						drawPixel(x + count, y + (rowCount * 8) + colIndex, !getInvertFont());
					}
					else
					{
						drawPixel(x + count, y + (rowCount * 8) + colIndex, getInvertFont());
					}
				}
			}
		}
	}
	else
	{
		fontIndex = ((value - _FontOffset) * ((_Font_X_Size * _Font_Y_Size) / 8)) + 4;
		colByte = *(_FontSelect + fontIndex);
		colbit = 7;
		for (cx = 0; cx < _Font_X_Size; cx++)
		{
			for (cy = 0; cy < _Font_Y_Size; cy++)
			{
				if ((colByte & (1 << colbit)) != 0)
				{
					drawPixel(x + cx, y + cy, !getInvertFont());
				}
				else
				{
					drawPixel(x + cx, y + cy, getInvertFont());
				}
				colbit--;
				if (colbit < 0)
				{
					colbit = 7;
					fontIndex++;
					colByte = *(_FontSelect + fontIndex);
				}
			}
		}
	}
	return 0;
}


uint8_t writeCharString(int16_t x, int16_t y, char *pText)
{
	uint8_t count = 0;
	uint8_t MaxLength = 0;
	// Check for null pointer
	if (pText == NULL)
	{
		printf("ERMCH1115_graphics::writeCharString Error 2 :String array is not valid pointer\n");
		return 2;
	}
	while (*pText != '\0')
	{
		// check if text has reached end of screen
		if ((x + (count * _Font_X_Size)) > _width - _Font_X_Size)
		{
			y = y + _Font_Y_Size;
			x = 0;
			count = 0;
		}
		if (writeChar(x + (count * (_Font_X_Size)), y, *pText++) != 0)
			return 3;
		count++;
		MaxLength++;
		if (MaxLength >= 150)
			break; // 2nd way out of loop, safety check
	}
	return 0;
}

size_t write(uint8_t character)
{
	switch (character)
	{
	case '\n':
		_cursor_y += _Font_Y_Size;
		_cursor_x = 0;
		break;
	case '\r':	
		break;
	default:
		if (writeChar(_cursor_x, _cursor_y, character) != 0)
			return -1;
		_cursor_x += (_Font_X_Size);
		if (_textwrap && (_cursor_x > (_width - (_Font_X_Size))))
		{
			_cursor_y += _Font_Y_Size;
			_cursor_x = 0;
		}
		break;
	} // end of switch

	return 1;
}
