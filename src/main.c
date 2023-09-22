#include <stdio.h>
#include "pico/stdlib.h"
#include "imu/icm2060.h"
#include "can/haltech.h"
#include "can/mcp2515.h"
#include "can/registers.h"
#include "hardware/uart.h"
int main() {

    

    // Initialize chosen serial port
    stdio_init_all();
    
    
    // Loop forever
    spi_init(spi0, 10000000); // 10MHZ clck
    
    gpio_set_function(2, GPIO_FUNC_SPI);
   
    gpio_set_function(3, GPIO_FUNC_SPI);
   
    gpio_set_function(4, GPIO_FUNC_SPI);
    
    spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    //CS select
    uart_init(uart0, 115200);

    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);

    uart_set_format(uart0, 8, 1, UART_PARITY_NONE);
    
    gpio_init(9);
    gpio_set_dir(9, GPIO_OUT);
     
    
    init(CAN_BUS_1000_16MHZ);

    canMessage * message = (canMessage*)malloc(sizeof(canMessage));
    
    message->ID = 0x7DF; //7DF
    message->DLC = 8;
    message->DATA[0] = 0x02;
    message->DATA[1] = 0x01;
    message->DATA[2] = 0x05;//0x05
    message->DATA[3] = 0x00;
    message->DATA[4] = 0x00;
    message->DATA[5] = 0x00;
    message->DATA[6] = 0x00;
    message->DATA[7] = 0x00; 
    printf("Start: \n");
    while (true) {
        canMessage * recieve = (canMessage*)malloc(sizeof(canMessage));
        read_message(recieve);
        printf("%2x\n", recieve->ID);
        haltech_can_data(recieve);
        free(recieve);
        //sleep_ms(10);
        
    }
}