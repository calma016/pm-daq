#ifndef _MCP2515_H 
#define _MCP2515_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "registers.h"
#include "stdio.h"

//Structure for CAN Messages
typedef struct{
    uint32_t ID; //ID
    uint8_t EID; //Extended ID
    uint8_t DLC; //DLC 
    uint8_t DATA[8];
}canMessage;

//Enumerated Values
typedef enum{
    CAN_SUCCESS,
    CAN_ERROR,
    CAN_UNKNOWN
}CAN_STATUS;
typedef enum{
    CAN_BUS_250_8MHZ,
    CAN_BUS_250_16MHZ,
    CAN_BUS_500_8MHZ,
    CAN_BUS_500_16MHZ,
    CAN_BUS_1000_8MHZ,
    CAN_BUS_1000_16MHZ 
}CAN_TIMING;

typedef enum{
    RXB0_M,
    RXB1_M
}RX_STATUS;
typedef enum{
    STATUS_RX0IF,
    STATUS_RX1IF,
    STATUS_TXREQ0,
    STATUS_TX01F,
    STATUS_TXREQ1,
    STATUS_TX1F,
    STATUS_TXREQ2,
    STATUS_TX2IF
}CANINTF_RESULT;


//Chip select
void cs_select();
void cs_deselect();
//SPI Instructions
uint8_t read_instruction(uint8_t address);
void write_instruction(uint8_t address, uint8_t data);
void write_instructions(uint8_t address, uint8_t data, int n);
uint8_t read_status_instruction();
void bit_modify_instruction(uint8_t address, uint8_t mask, uint8_t data);
uint8_t rx_status_instruction(); 

CAN_STATUS init(CAN_TIMING selection);
CAN_STATUS read_message(canMessage *message);
CAN_STATUS write_message(canMessage *message);




#endif 
