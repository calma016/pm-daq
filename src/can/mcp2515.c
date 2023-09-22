#include "mcp2515.h"

//Chip select activate
void cs_select(){
    
        gpio_put(9, 0);
    
    
}
//Chip select deactivate
void cs_deselect(){
    
        gpio_put(9, 1);
    
}
//Read instrution MCP2515
uint8_t read_instruction(uint8_t address){

    uint8_t buffer[2] = {(uint8_t)SPI_READ, address};    
    uint8_t  result = 0;
    cs_select();
    spi_write_blocking(spi0, buffer, 2);
    spi_read_blocking(spi0, 0, &result, 1);
    cs_deselect();
    return result;
}

//Write instructions for MCP2515
void write_instruction(uint8_t address, uint8_t data){
    uint8_t buffer[3] = {(uint8_t)SPI_WRITE, address, data};
    cs_select();
    spi_write_blocking(spi0, buffer, 3);
    cs_deselect();
}

void write_instructions(uint8_t address, uint8_t data, int n){
    uint8_t buffer[3] = {(uint8_t)SPI_WRITE, address, data};
    for(uint8_t i = 0; i < n; i++){
        buffer[1] += i;
        cs_select();
        spi_write_blocking(spi0, buffer, 3);
        cs_deselect();
    }
}
//Status Instructions for MCP2515
uint8_t read_status_instruction(){

    uint8_t buffer[1] = {(uint8_t)SPI_STATUS};
    uint8_t result = 0;
    cs_select();
    spi_write_blocking(spi0, buffer, 1);
    spi_read_blocking(spi0, 0, &result, 1);
    cs_deselect();
    return result;
}

void bit_modify_instruction(uint8_t address, uint8_t mask, uint8_t data){
    uint8_t buffer[4] = {(uint8_t)SPI_MODIFY, address, mask, data};
    cs_select();
    spi_write_blocking(spi0, buffer, 4);
    cs_deselect();
}
uint8_t rx_status_instruction(){
    uint8_t buffer[1] = {(uint8_t)SPI_RX_STATUS};
    cs_select();
    spi_write_blocking(spi0, buffer, 1);
    uint8_t result = 0;
    spi_read_blocking(spi0, 0, &result, 1);
    cs_deselect();
    return result;
}
CAN_STATUS init(CAN_TIMING selection){

    //Configure morde REQOP[2:0] = 100
    
    //initialize spi bus 
    

    //Set divice in configuration mode 
    write_instruction(CANCTRL, (1 << REQOP2));

    uint8_t verify_canctrl = read_instruction(CANCTRL);
    printf("CANCTRL: %2x\n", verify_canctrl);

    switch(selection){
        case CAN_BUS_250_8MHZ:
            write_instruction(CNF1, 0x00);
            write_instruction(CNF2, 0x9a);
            write_instruction(CNF3, 0x07);
            break;
        case CAN_BUS_250_16MHZ:
            write_instruction(CNF1, 0x41);
            write_instruction(CNF2, 0xF1);
            write_instruction(CNF3, 0x85);
            break;
        case CAN_BUS_500_8MHZ:
            write_instruction(CNF1, 0x00);//40
            write_instruction(CNF2, 0xF0);//e5
            write_instruction(CNF3, 0x86);//83
            break;
        case CAN_BUS_500_16MHZ:
            write_instruction(CNF1, 0x00);
            write_instruction(CNF2, 0x9a);
            write_instruction(CNF3, 0x07);
            break;
        case CAN_BUS_1000_8MHZ:
            write_instruction(CNF1, 0x00);
            write_instruction(CNF2, 0x00);
            write_instruction(CNF3, 0x00);
            break;
        case CAN_BUS_1000_16MHZ:
            write_instruction(CNF1, 0x00);
            write_instruction(CNF2, 0xD0);
            write_instruction(CNF3, 0x82);
            break;
    }

    

    //Pin Control and status register
    write_instruction(TXRTSCTRL, 0x00);
    
    //Clear out the recieve and transfer buffers
    write_instruction(TXB0CTRL, 0x00); write_instruction(TXB1CTRL, 0x00); write_instruction(TXB2CTRL, 0x00);
    write_instruction(RXB0CTRL, 0x00); write_instruction(RXB1CTRL, 0x00);
    
    //Clear masks and filters
    write_instructions(RXF0SIDH, 0x00, 4); write_instructions(RXF1SIDH, 0x00, 4);
    write_instructions(RXF3SIDH, 0x00, 4); write_instructions(RXF4SIDH, 0x00, 4);

    write_instructions(RXM0SIDH, 0x00, 4); write_instructions(RXM1SIDH, 0x00, 4);

    //Recieve any messages on both buffers
    write_instruction(RXB0CTRL, (1 << RXM1) | (1 << RXM0));
    write_instruction(RXB1CTRL, (1 << RXM1) | (1 << RXM0));
    uint8_t verify_rxb = read_instruction(RXB0CTRL);
    

    //Normal Operations
    write_instruction(CANCTRL, 0x00);
    sleep_ms(100);


    return CAN_SUCCESS;
}



CAN_STATUS write_message(canMessage *message){

    if(((read_instruction(TXB0CTRL) & (0x04)) >> 2)== 0){   

        
        //Load registers to transmit messages

        //Load ID
        write_instruction(TXB0SIDH, (uint8_t)(message->ID >> 3));
        write_instruction(TXB0SIDL, (uint8_t)((message->ID & 0x07) << 5));
        write_instruction(TXB0DLC,  message->DLC);
        write_instruction(TXB0D0, message->DATA[0]);
        write_instruction(TXB0D1, message->DATA[1]);
        write_instruction(TXB0D2, message->DATA[2]);
        write_instruction(TXB0D3, message->DATA[3]);
        write_instruction(TXB0D4, message->DATA[4]);
        write_instruction(TXB0D5, message->DATA[5]);
        write_instruction(TXB0D6, message->DATA[6]);
        write_instruction(TXB0D7, message->DATA[7]);

        //Send message transmission
        write_instruction(TXB0CTRL, (1 << TXREQ) | (1 << TXP1) | ( 1 << TXP0));
        //Modify the TXBnCTRL register
        if(read_instruction(TXB0CTRL) & ( 1 << ABTF | 1 << MLOA | 1 << TXERR) != 0)
            return CAN_ERROR;
    }else if(((read_instruction(TXB1CTRL) & 0x04) >> 2) == 0){

        printf("TXB2 \n");
        write_instruction(TXB1SIDH, message->ID >> 3);
        write_instruction(TXB1SIDL, message->ID & 0x0F);
        write_instruction(TXB1DLC, message->DLC);
        //Load Data
        write_instruction(TXB1D0, message->DATA[0]);
        write_instruction(TXB1D1, message->DATA[1]);
        write_instruction(TXB1D2, message->DATA[2]);
        write_instruction(TXB1D3, message->DATA[3]);
        write_instruction(TXB1D4, message->DATA[4]);
        write_instruction(TXB1D5, message->DATA[5]);
        write_instruction(TXB1D6, message->DATA[6]);
        write_instruction(TXB1D7, message->DATA[7]);

        //Send message transmission
        write_instruction(TXB1CTRL, (1 << TXREQ) | (1 << TXP1) | ( 1 << TXP0));
        if(read_instruction(TXB1CTRL) & ((1 << ABTF) | (1 << MLOA) | (1 << TXERR)) != 0)
            return CAN_ERROR;

    }else if((read_instruction(TXB2CTRL) & 0x04) >> 2 == 0){
        printf("TXB3 \n");
        write_instruction(TXB2SIDH, message->ID  >> 3);
        write_instruction(TXB2SIDL, message->ID & 0x0F);
        write_instruction(TXB2DLC, message->DLC);
        //Load Data
        write_instruction(TXB2D0, message->DATA[0]);
        write_instruction(TXB2D1, message->DATA[1]);
        write_instruction(TXB2D2, message->DATA[2]);
        write_instruction(TXB2D3, message->DATA[3]);
        write_instruction(TXB2D4, message->DATA[4]);
        write_instruction(TXB2D5, message->DATA[5]);
        write_instruction(TXB2D6, message->DATA[6]);
        write_instruction(TXB2D7, message->DATA[7]);

        //Send message transmission
        write_instruction(TXB2CTRL, (1 << TXREQ) | (1 << TXP1) | ( 1 << TXP0));
        if(read_instruction(TXB2CTRL) & ((1 << ABTF) | (1 << MLOA) | (1 << TXERR))  != 0)
            return CAN_ERROR;

    }
    return CAN_SUCCESS;
}
//TODO: add the free the bit from CANINFE
CAN_STATUS read_message(canMessage *message){
    uint8_t rx_status_reg = rx_status_instruction();

    switch((rx_status_reg & 0xC0) >> 6){
        case 0x01:
        {   
            //Extended Data Frame bit
            bool IDE = read_instruction(RXB0SIDL) & (1 << 3);
            message->ID = (read_instruction(RXB0SIDH) << 3) + (read_instruction(RXB0SIDL) >> 5);
            

            //Data Length Code bits
            uint8_t DLC = (read_instruction(RXB0DLC) & 0x0F); 
            if(DLC > 0x0F){
                return CAN_ERROR;
            }

            message->DATA[0] = read_instruction(RXB0D0);
            message->DATA[1] = read_instruction(RXB0D1);
            message->DATA[2] = read_instruction(RXB0D2);
            message->DATA[3] = read_instruction(RXB0D3);
            message->DATA[4] = read_instruction(RXB0D4);
            message->DATA[5] = read_instruction(RXB0D5);
            message->DATA[6] = read_instruction(RXB0D6);
            message->DATA[7] = read_instruction(RXB0D7);

            bit_modify_instruction(CANINTF, 0x1, 0);


        }
        break;
        case 0x02:
        {
            printf("Hello\n");
            /*
            
            //Extended data frame bit
            bool IDE = read_instruction(RXB1CTRL) & 0x04;
            message->ID = read_instruction(RXB1SIDH) << 3 | read_instruction(RXB1SIDL) & 0xE0 >> 5;

            uint8_t DLC = read_instruction(RXB1DLC) & 0x0F;
            if(DLC > 0x0F)
                return CAN_ERROR;

            message->DATA[0] = read_instruction(RXB1D0);
            message->DATA[1] = read_instruction(RXB1D1);
            message->DATA[2] = read_instruction(RXB1D2);
            message->DATA[3] = read_instruction(RXB1D3);
            message->DATA[4] = read_instruction(RXB1D4);
            message->DATA[5] = read_instruction(RXB1D5);
            message->DATA[6] = read_instruction(RXB1D6);
            message->DATA[7] = read_instruction(RXB1D7);
            */
           
        }
        break;
    }
    return CAN_SUCCESS;
}