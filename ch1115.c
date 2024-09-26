#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "RP2040.h"
#include "ch1115.h"
#include "ch1115_graphics.h"


uint8_t OLEDbuffer[1024];

void ch1115_reset(void){
    gpio_put(PIN_RST, 1);
    sleep_ms(10);
    gpio_put(PIN_RST, 0);
    sleep_ms(10);
    gpio_put(PIN_RST, 1);
    sleep_ms(100);
}

static inline void ch1115_cs_select(void){
	asm volatile("nop \n nop \n nop");
	gpio_put(PIN_CS,0);
	asm volatile("nop \n nop \n nop");
}

static inline void ch1115_cs_deselect(void){
	asm volatile("nop \n nop \n nop");
	gpio_put(PIN_CS,1);
	asm volatile("nop \n nop \n nop");
}

static inline void ch1115_dc_select(void){
	asm volatile("nop \n nop \n nop");
	gpio_put(PIN_DC,0);
	asm volatile("nop \n nop \n nop");
}
static inline void ch1115_dc_deselect(void){
	asm volatile("nop \n nop \n nop");
	gpio_put(PIN_DC,1);
	asm volatile("nop \n nop \n nop");
}

void ch1115_data(uint8_t data){
	ch1115_cs_select();
    spi_write_blocking(SPI_PORT, &data, 1);
	ch1115_cs_deselect();
}

void ch1115_command(uint8_t command, uint8_t value){
	ch1115_cs_select();
    ch1115_dc_select();
    ch1115_data(command | value);
    ch1115_dc_deselect();
	ch1115_cs_deselect();
}

int ch1115_init(void){
    
    ch1115_reset();

	// ch1115_cs_select();

    ch1115_command(ERMCH1115_DISPLAY_OFF, 0);

	ch1115_command(ERMCH1115_SET_COLADD_LSB, 0);
	ch1115_command(ERMCH1115_SET_COLADD_MSB, 0);
	ch1115_command(ERMCH1115_SET_PAGEADD, 0);
	ch1115_command(ERMCH115_SET_DISPLAY_START_LINE, 0);

	ch1115_command(ERMCH115_CONTRAST_CONTROL, 0);
	ch1115_command(0x80, 0);

	ch1115_command(ERMCH1115_IREF_REG, 0);
	ch1115_command(ERMCH1115_IREF_SET, 0);

	ch1115_command(ERMCH1115_SEG_SET_REMAP, 0);
	ch1115_command(ERMCH1115_SEG_SET_PADS, 0);
	ch1115_command(ERMCH1115_ENTIRE_DISPLAY_ON, 0);
	ch1115_command(ERMCH1115_DISPLAY_NORMAL, 0);

	ch1115_command(ERMCH1115_MULTIPLEX_MODE_SET, 0);
	ch1115_command(ERMCH1115_MULTIPLEX_DATA_SET, 0);

	ch1115_command(ERMCH1115_COMMON_SCAN_DIR, 0);

	ch1115_command(ERMCH1115_OFFSET_MODE_SET, 0);
	ch1115_command(ERMCH1115_OFFSET_DATA_SET, 0);

	ch1115_command(ERMCH1115_OSC_FREQ_MODE_SET, 0);
	ch1115_command(ERMCH1115_OSC_FREQ_DATA_SET, 0);

	ch1115_command(ERMCH1115_PRECHARGE_MODE_SET, 0);
	ch1115_command(ERMCH1115_PRECHARGE_DATA_SET, 0);

	ch1115_command(ERMCH1115_COM_LEVEL_MODE_SET, 0);
	ch1115_command(ERMCH1115_COM_LEVEL_DATA_SET, 0);

	ch1115_command(ERMCH1115_SET_PUMP_REG, ERMCH115_SET_PUMP_SET);

	ch1115_command(ERMCH1115_DC_MODE_SET, 0);
	ch1115_command(ERMCH1115_DC_ONOFF_SET, 0);

	ch1115_command(ERMCH1115_DISPLAY_ON, 0);

    // ch1115_cs_deselect();

    sleep_ms(100);
    return 1;
}

void OLEDBufferScreen(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t *data)
{
	// gpio_put(PIN_CS, 0);

	uint8_t tx, ty;
	uint16_t offset = 0;
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y / 8;

	for (ty = 0; ty < h; ty = ty + 8)
	{
		if (y + ty < 0 || y + ty >= 64)
		{
			continue;
		}

		ch1115_command(ERMCH1115_SET_COLADD_LSB, (column & 0x0F));
		ch1115_command(ERMCH1115_SET_COLADD_MSB, (column & 0XF0) >> 4);
		ch1115_command(ERMCH1115_SET_PAGEADD, page++);

		for (tx = 0; tx < w; tx++)
		{
			if (x + tx < 0 || x + tx >= 128)
			{
				continue;
			}
			offset = (w * (ty / 8)) + tx;
			ch1115_data(data[offset++]);
		}
	}
	// gpio_put(PIN_CS, 1);
}

void OLEDupdate()
{
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t w = 128;
	uint8_t h = 64;
	OLEDBufferScreen(x, y, w, h, OLEDbuffer);
}


void drawPixel(int16_t x, int16_t y, uint8_t colour)
{
	if ((x < 0) || (x >= 128) || (y < 0) || (y >= 64))
	{
		return;
	}
	int16_t temp;
	uint8_t RotateMode = 0;
	switch (RotateMode) {
	case 1:
		temp = x;
		x = _width - 1 - y;
		y = temp;
	break;
	case 2:
		x = _width - 1 - x;
		y = _height - 1 - y;
	break;
	case 3:
		temp = x;
		x = y;
		y = _height - 1 - temp;
	break;
	}
	uint16_t tc = (_width * (y / 8)) + x;
	switch (colour)
	{
	case WHITE:
		OLEDbuffer[tc] |= (1 << (y & 7));
		break;
	case BLACK:
		OLEDbuffer[tc] &= ~(1 << (y & 7));
		break;
	case INVERSE:
		OLEDbuffer[tc] ^= (1 << (y & 7));
		break;
	}
}

void drawCircle(int16_t x0, int16_t y0, int16_t r,
									uint8_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	drawPixel(x0, y0 + r, color);
	drawPixel(x0, y0 - r, color);
	drawPixel(x0 + r, y0, color);
	drawPixel(x0 - r, y0, color);

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		drawPixel(x0 + x, y0 + y, color);
		drawPixel(x0 - x, y0 + y, color);
		drawPixel(x0 + x, y0 - y, color);
		drawPixel(x0 - x, y0 - y, color);
		drawPixel(x0 + y, y0 + x, color);
		drawPixel(x0 - y, y0 + x, color);
		drawPixel(x0 + y, y0 - x, color);
		drawPixel(x0 - y, y0 - x, color);
	}
}