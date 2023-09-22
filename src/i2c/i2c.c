#include "i2c.h"

void i2c_setup(){
    //Initialize I2C
    i2c_init(i2c1, 400*1000);
    //Set I2C pins
    #ifdef I2C_PROG_SCL
        gpio_set_function(I2C_PROG_SCL, GPIO_FUNC_I2C);
    #endif
    #ifdef I2C_PROG_SDA
        gpio_set_function(I2C_PROG_SDA, GPIO_FUNC_I2C);
    #endif 
}