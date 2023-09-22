#include "icm2060.h"

IMU_STATUS device_check(){
    //Store data in buffer
    uint8_t buffer[1] = {WHO_AM_I};
    static uint8_t addr = DEVICE_ADDRESS;
    
    i2c_write_blocking(i2c1, addr, buffer, 1, true); //Write Trans.
    uint8_t result = 0;
    i2c_read_blocking(i2c1, addr, &result, 1, false); //Read Trans
    if(buffer[0] != 0x11) //Verify register values
        return IMU_ERROR;
    printf("%2x\n", buffer[0]);
    return IMU_SUCCESS;
}
IMU_STATUS power_management(){
    uint8_t buffer[2] = {PWR_MGMT_1, (0x01)};
    static uint8_t addr = DEVICE_ADDRESS;
    i2c_write_blocking(i2c1, addr, buffer, 2, false);

    

    buffer[0] = PWR_MGMT_2; buffer[1] = 0x00;
    i2c_write_blocking(i2c1, addr, buffer, 2, false);

    uint8_t check_buffer[1] = {PWR_MGMT_2};
    uint8_t result = 0;
    i2c_write_blocking(i2c1, addr, check_buffer, 1, true);
    i2c_read_blocking(i2c1, addr, &result, 1, false);
    if(((result & 0x80) >> 7) != 0){
        return IMU_ERROR;
    }
    return IMU_SUCCESS;
}
IMU_STATUS initialize(){
    static uint8_t addr = DEVICE_ADDRESS;
    if(power_management() == IMU_ERROR){
        return IMU_ERROR;
    }
    //Accel Configuration 
    uint8_t buffer[2] = {ACCEL_CONFIG, ( 1 << 7) | (1 << 6) | (1 << 6) | (1 << 3)};
    i2c_write_blocking(i2c1, addr, buffer, 2, false);
    buffer[0] = ACCEL_CONFIG_TWO; buffer[1] = (1 << 6);
    i2c_write_blocking(i2c1, addr, buffer, 2, false);

    //Gyro configuration
    buffer[0] = GYRO_CONFIG; buffer[1] = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 3);
    i2c_write_blocking(i2c1, addr, buffer, 2, false);
    buffer[0] = LP_MODE_CFG; buffer[1] = 0x00;
    i2c_write_blocking(i2c1, addr, buffer, 2, false);
    return IMU_SUCCESS;
}
IMU_STATUS raw_gyro(uint16_t *rawGyro){
    /*
        GYRO_OUT = Gyro_Sensistivity * X_angular_rate
        Nominal FS_SEL = 0
        Conditions Gyro_Sensistivity = 131 LSB/(dps)
    */
    static uint8_t addr = DEVICE_ADDRESS;
    uint8_t reg = GYRO_XOUT_H;
    uint8_t buffer[6];
    i2c_write_blocking(i2c1, addr, &reg, 1, true);
    i2c_read_blocking(i2c1, addr, buffer, 6, false);
    for(uint8_t itr = 0; itr < 3; itr++){
        rawGyro[itr] = (buffer[itr*2 + 1] << 8) | buffer[itr*2];
    }
    return IMU_SUCCESS;
}
IMU_STATUS raw_accel(uint16_t *rawAccel){
    static uint8_t addr = DEVICE_ADDRESS;
    uint8_t reg = ACCEL_XOUT_H;
    uint8_t buffer[6];
    i2c_write_blocking(i2c1, addr, &reg, 1, true);
    i2c_read_blocking(i2c1, addr, buffer, 6, false);
    for(uint8_t itr = 0; itr < 3; itr++){
        rawAccel[itr] = (buffer[itr*2 + 1] << 8) | buffer[itr*2];
    }
    return IMU_SUCCESS;
}
IMU_STATUS dc_bias_offset_gyro(uint16_t* dcOffsetGyro){
    static uint8_t addr = DEVICE_ADDRESS;
    uint8_t reg = XG_OFFS_URSH;
    uint8_t buffer[6];
    i2c_write_blocking(i2c1, addr, &reg, 1, true);
    i2c_read_blocking(i2c1, addr, buffer, 6, false);
    for(uint8_t itr = 0; itr < 3; itr++){
        dcOffsetGyro[itr] = ((buffer[itr*2+1] << 8) | buffer[itr*2]);
    }
    return IMU_SUCCESS;
}
IMU_STATUS temperature_bias_offset_gyro(uint16_t* tempOffsetGyro){

    //TODO:FIX incorrect bias extraction
    static uint8_t addr = DEVICE_ADDRESS;
    uint8_t reg = XG_OFFS_TC_H;
    uint8_t buffer[6];
    i2c_write_blocking(i2c1, addr, &reg, 1, true);
    i2c_read_blocking(i2c1, addr, buffer, 6, false);
    for(uint8_t itr = 0; itr < 3; itr++){
        tempOffsetGyro[itr] = ((buffer[itr*2+1] << 8) | buffer[itr*2]);
    }
    return IMU_SUCCESS;
}
