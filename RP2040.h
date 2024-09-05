// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   13
#define PIN_SCK  10
#define PIN_MOSI 11

#define PIN_DC   14
#define PIN_RST   9
#define PIN_LED  27