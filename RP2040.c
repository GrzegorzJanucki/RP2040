#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "pico/util/queue.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "RP2040.h"
#include "ch1115.h"
#include "ch1115_graphics.h"
#include "ch1115_font.h"
#include "ch1115_print.h"
#include "jFont.h"
#include "jLabel.h"
#include "jText.h"


int ch1115_init(void);

int sample_result = 0; //symulowany wynik do wyświetenia
bool result_timer_callback(repeating_timer_t *rt);
repeating_timer_t result_timer;
queue_t result_fifo;


bool oled_timer_callback(repeating_timer_t *rt);
repeating_timer_t oled_timer;
queue_t oled_fifo;
bool dot = true;


int main()
{
    stdio_init_all();


    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 5*1000*1000);
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


    CH1115_graphics_init();

    adc_init();
    adc_gpio_init(29);
    adc_select_input(3);
    
    point2D_t p = {.x=5, .y=15};
    label_t* pLabel1 = CreateLabel(p, &FontDigital32Info, "-123.45");
    txt_wnd_t* pTxt1 = CreateTxtWnd(p, 100, 32, &FontDigital32Info, 7);
    txt_wnd_t* pTxt2 = CreateTxtWnd(p, 25, 21, &FontDigital18Info, 2);


    pTxt1->just = right;
    pTxt2->just = left;


    p.x=64; p.y=15;
    MoveTextWnd(pTxt1,p,topright);
    p.x=65; p.y=pTxt1->height +15;
    MoveTextWnd(pTxt2,p,botleft);
    
    queue_init(&result_fifo, sizeof(int), 4);  
    add_repeating_timer_ms(50, result_timer_callback, NULL, &result_timer);


    queue_init(&oled_fifo, sizeof(bool), 4);  
    add_repeating_timer_ms(330, oled_timer_callback, NULL, &oled_timer);


    int iResult;
    bool bDot;
    char buff[10];


    setFont(pFontDefault);
    setCursor(15, 5);
    char nazwa[] = "Power Meter v0.01\n";
    println1(nazwa);
    
    const float conversion_factor = 3.3f / (1 << 12);

    while (true) {
        if(queue_try_remove(&result_fifo, &iResult)){
            uint16_t result = adc_read();
            printf("result=%d\n",result);
                                                //uint16_t result = adc_read();
            // iResult = iResult%1000;
            /* if(iResult%2)
                ShowLabel(pLabel1);
            else
                HideLabel(pLabel1); */
            
            ClearTextWnd(pTxt1);
            //sprintf(buff, "%d.", iResult/100);   
            sprintf(buff, "%4.1f", result*conversion_factor);
            SetStringDataTxtWnd(pTxt1, buff);
            ShowTextWnd(pTxt1);

            /*
            ClearTextWnd(pTxt2);
            sprintf(buff, "%02d", iResult - 100*(iResult/100));
            SetStringDataTxtWnd(pTxt2, buff);
            ShowTextWnd(pTxt2);
            */

            OLEDupdate();
        }


        // char wynik[] ="0123.45";
        // WriteCharFontNew(10, 10, '0', &FontDigitalnfo);
        // PrintStringFont(10, 20, wynik, &FontDigitalnfo);

        if(queue_try_remove(&oled_fifo, &bDot)){
            if(bDot)
                fillCircle(5, 7, 2, WHITE);
            else
                fillCircle(5, 7, 2, BLACK);
            OLEDupdate();
        }
    }
}




bool result_timer_callback(repeating_timer_t *rt) {
    sample_result++;
    if (!queue_try_add(&result_fifo, &sample_result)) {
        printf("FIFO was full\n");
    }
    return true; // keep repeating
}


bool oled_timer_callback(repeating_timer_t *rt){
    if (!queue_try_add(&oled_fifo, &dot)) {
        printf("FIFO was full\n");
    }
    dot = !dot;
    return true; // keep repeating
}

