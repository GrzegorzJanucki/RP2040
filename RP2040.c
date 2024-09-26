#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "RP2040.h"
#include "ch1115.h"
#include "ch1115_graphics.h"
#include "ch1115_font.h"
#include "ch1115_print.h"

int ch1115_init(void);

int main()
{
    stdio_init_all();

    // gpio_init(PIN_SCK);
	// gpio_init(PIN_MOSI);
    // gpio_init(PIN_MISO);

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1*1000*1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    // gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    bi_decl(bi_3pins_with_func(PIN_MISO, PIN_MOSI, PIN_SCK, GPIO_FUNC_SPI));

    // spi_set_format(SPI_PORT, // SPI instance
	// 			   8,			 // Number of bits per transfer
	// 			   SPI_CPOL_0,	 // Polarity (CPOL)
	// 			   SPI_CPHA_0,	 // Phase (CPHA)
	// 			   SPI_MSB_FIRST);
    
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);

    bi_decl(bi_1pin_with_name(PIN_CS,"SPI_CS"));

    gpio_init(PIN_RST);
    gpio_set_dir(PIN_RST, GPIO_OUT);
    gpio_put(PIN_RST, 1);

    gpio_init(PIN_DC);
    gpio_set_dir(PIN_DC, GPIO_OUT);
    gpio_put(PIN_DC, 1); 
    
    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);
    gpio_put(PIN_LED, 1); 


    printf("start ch1115_init");
    ch1115_init();
    printf("end ch1115_init");
    
    for(int i = 0; i<2; i++)
        sleep_ms(1000);

    

   // drawPixel(20, 20, 1);
    //drawCircle(60, 30, 15, 1);
    CH1115_graphics_init();
    setCursor(60, 15);
    setFont(pFontDefault);
    print('x');

    while (true) {
        printf("Hi Grzesiek!\n");
        gpio_put(PIN_LED, 0);
        sleep_ms(500);
        OLEDupdate();
        gpio_put(PIN_LED, 1);
        sleep_ms(500);
    }
}
