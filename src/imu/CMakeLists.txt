
add_library(imu
    icm2060.h 
    icm2060.c
    registers.h
)

target_link_libraries(imu
    pico_stdlib 
    hardware_i2c
)

target_include_directories(imu PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")