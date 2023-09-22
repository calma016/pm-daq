#ifndef _IMU_H
#define _IMU_H 


//Libraries
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "stdlib.h"
#include "stdio.h"
#include "registers.h"

typedef enum {
    IMU_SUCCESS,
    IMU_ERROR,
    IMU_UNKNOWN
}IMU_STATUS;

IMU_STATUS device_check();
IMU_STATUS power_management();
IMU_STATUS initialize();
IMU_STATUS raw_accel(uint16_t *rawAccel);
IMU_STATUS raw_gyro(uint16_t *rawGyro);
IMU_STATUS dc_bias_offset_gyro(uint16_t* dcOffsetGyro);
IMU_STATUS temperature_bias_offset_gyro(uint16_t* tempOffsetGyro);




#endif 