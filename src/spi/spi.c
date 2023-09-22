#include "spi.h"

void spi_setup(){
    spi_init(spi0, 10000000); //10 MHZ baud rate

    #ifdef SPI_PROG_SCK
        gpio_set_function(SPI_PROG_SCK, GPIO_FUNC_SPI);
    #endif
    #ifdef SPI_PROG_MISO
        gpio_set_function(SPI_PROG_MISO, GPIO_FUNC_SPI);
    #endif
    #ifdef SPI_PROG_MOSI
        gpio_set_function(SPI_PROG_MISO, GPIO_FUNC_SPI);
    #endif
    #ifdef SPI_PROG_CSN
        gpio_init(SPI_PROG_CSN);
        gpio_set_dir(SPI_PROG_CSN, GPIO_OUT);
    #endif
}